#!/bin/bython
import subprocess 
import time 
import sys 
import re 

class GDB :
    proc =None ;
    def __init__ (self ,command ):
        self .proc =subprocess .Popen (command ,stdout =subprocess .PIPE ,stdin =subprocess .PIPE );
        
    def write (self ,message ):
        self .proc .stdin .write (message .encode ("utf-8"));
        self .proc .stdin .flush ();
        #print(message);
        
    def writeln (self ,message ):
        self .write (message +"\n");
        
    def readline (self ):
        line ="";
        line +=self .proc .stdout .readline ().decode ("utf-8");
        
    def read (self ):
        content =""
        while (not content .endswith ("(gdb)")):
            content +=self .proc .stdout .read (1).decode ("utf-8")
            
        return content ;
        
    

def get_lines_by_prefix (out ,prefix ):
    lines =[]
    for i in out .split ("\n"):
        if (i .startswith (prefix )):
            lines .append (i );
            
        
    return lines ;
    

def struct_info (gdb ,struct_type ):
    props =[]
    gdb .writeln ("ptype "+struct_type )
    #retrieve all variables inside a containing struct
    struct_types =get_lines_by_prefix (gdb .read (),"~")[1:-1]
    
    for i ,j in enumerate (struct_types ):
        #j is \t* int *data;\\n;
        vname =j .split (" ")[-1].split ("*")[-1][0:-4]
        vtype =" ".join (j .split (" ")[4:-1])
        vtype +=" "+"*"*(len (j .split ("*"))-1)
        props .append ([vtype ,vname ]);
        
    return props ;
    

def var_info (gdb ,varname ):
    
    #~"$4 = (Node *) 0x5555555592a0\n"
    gdb .writeln ("p "+varname );
    memdata =get_lines_by_prefix (gdb .read (),"~")[0];
    
    #~"$2 = (int *) 0x7fffffffe610\n"
    gdb .writeln ("p &"+varname );
    addressdata =get_lines_by_prefix (gdb .read (),"~")[0];
    
    return (
    addressdata .split (" ")[-1][0:-3],#address
    addressdata .split ("(")[1].split (")")[0][0:-1],#type
    memdata .split (" ")[-1][0:-3]#value
    );
    

def free_var (gdb ,memory ,varname ):
    var =var_info (gdb ,varname )
    struct =struct_info (gdb ,var [1])
    for i in struct :
        gdb .writeln ("p &"+varname +"->"+i [1])
        address =get_lines_by_prefix (gdb .read (),"~")[0].split (" ")[-1][0:-3]
        del memory [0][address ]
        
    

#specifically for heap memory and structs and shit
def memcrawl (gdb ,memory ,variable ):
    #memory_object is an array: [type, address]
    gdb .writeln ("p &("+variable +")")
    info =get_lines_by_prefix (gdb .read (),"~")[0];
    #retrieve type from $2 = (int *) 0xFFFFFFFFFFFF\n
    vartype =info .split ("(")[1].split (")")[0][0:-1]
    
    gdb .writeln ("ptype "+vartype )
    #retrieve all variables inside a containing struct
    struct_types =get_lines_by_prefix (gdb .read (),"~")[1:-1]
    
    for i ,j in enumerate (struct_types ):
        #j is \t* int *data;\\n;
        vname =j .split (" ")[-1].split ("*")[-1][0:-4]
        vtype =" ".join (j .split (" ")[4:-1])
        vtype +=" "+"*"*(len (j .split ("*"))-1)
        gdb .writeln ("p "+variable +"->"+vname )
        value =get_lines_by_prefix (gdb .read (),"~")[0].split (" ")[-1][0:-3];
        gdb .writeln ("p &"+variable +"->"+vname )
        address =get_lines_by_prefix (gdb .read (),"~")[0].split (" ")[-1][0:-3];
        
        memory [0][address ]=[vtype ,value ];
        
    
    
    return memory 
    

def map_memory (gdb ,mem ):
    memory =mem [0];
    variables =mem [1];
    gdb .writeln ("info local");
    out =gdb .read ();
    memoryLines =get_lines_by_prefix (out ,"~");
    for i in memoryLines :
        varname =i .split ("=")[0][2:-1];
        info =var_info (gdb ,varname );
        variables [varname ]=info [0]
        memory [info [0]]=[info [1],info [2]]
        
    for i ,j in enumerate (memory ):
        if i <len (memoryLines ):
            continue ;
            
        gdb .writeln ("p *"+j );
        value =get_lines_by_prefix (gdb .read (),"~")[0];
        #format is ~"$n = 5\n"
        memory [j ]=[memory [j ][0],value .split (" ")[-1][0:-3]]
        
    return (memory ,variables )
    


def main ():
    #load file and stuff
    print ("Enter Input File\n")
    filename =input ("> ")
    gdb =GDB (["gdb","--interpreter=mi",filename ]);
    gdb .read ()
    gdb .writeln ("break main");
    gdb .read ()
    gdb .writeln ("r");
    gdb .read ()
    gdb .read ()#second one here is needed
    lines =["","",""]
    memory =(dict (),dict ());
    
    while True :
        gdb .writeln ("n");
        gdb .read ();
        lines .append (get_lines_by_prefix (gdb .read (),"~")[0][2:-3]);
        
        #remove preceeding integers from line
        lines [-1]=lines [-1][len (re .findall (r"^[0-9]*",lines [-1])[0]):]
        lines [-1]=re .sub (r"\\t","",lines [-1])
        
        memory =map_memory (gdb ,memory );
        
        print ("\n||||||||||||||||||||||||||||||||||||||||||\n");
        if (re .search (r"^\s*.*= (\(.*\)|)malloc\(.*\)",lines [-2])):
            #retrieve variable name that was malloc'd to from the line
            variable =lines [-2].split ("=")[0].split (" ");
            if variable [-1]=='':
                variable =variable [-2]
                
            else :
                variable =variable [-1]
                
            memory =memcrawl (gdb ,memory ,variable );
            
            print ("Malloc Found: "+variable )
            
        elif (re .search (r"^( |\t|\\t)*free\(.*\)",lines [-2])):
            variable =re .sub (r"^.*free\(","",lines [-2],count =1)
            variable =variable [0:-2]
            free_var (gdb ,memory ,variable );
            print ("Free found: "+variable )
            
        else :
            print ("")
            
        
        print ("Memory: \n")
        for i in memory [0]:
            print (i ,memory [0][i ]);
            
        print ("\nVariables: -----------------\n");
        for i in memory [1]:
            print (i ,memory [1][i ]);
            
        print ("\nLines: ----------------------\n");
        print ("p "+lines [-3])
        print ("r "+lines [-2])
        print ("n "+lines [-1])
        input ("")
        
    

if (__name__ =="__main__"):
    main ();
    

