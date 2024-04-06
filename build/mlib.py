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
    

def get_lines_by_prefix (out ,prefix ):
    lines =[]
    for i in out .split ("\n"):
        if (i .startswith (prefix )):
            lines .append (i );
            
        
        
    
    return lines ;
    


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
    memoryLines =[]
    memoryLines =get_lines_by_prefix (out ,"~");
    for i in memoryLines :
        varname =i .split ("=")[0][2:-1];
        try :
            info =var_info (gdb ,varname );
            
        except :
            return (memory ,variables )
            
        
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
    

