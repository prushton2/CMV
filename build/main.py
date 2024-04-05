#!/bin/bython
import time 
import sys 
import re 

from GDB import GDB 
from mlib import *
import urwid 

def update (gdb ,memory ,lines ):
    gdb .read ()
    out =gdb .read ();
    #   print(out)
    lines .append (get_lines_by_prefix (out ,"~")[0][2:-3])
    
    #remove preceeding integers from line
    lines [-1]=lines [-1][len (re .findall (r"^[0-9]*",lines [-1])[0]):]
    lines [-1]=re .sub (r"\\t","",lines [-1])
    
    try :
        memory =map_memory (gdb ,memory );
        
    
    except ():
        return ;
        
    
    
    if (re .search (r"^\s*.*= (\(.*\)|)malloc\(.*\)",lines [-2])):
        #retrieve variable name that was malloc'd to from the line
        variable =lines [-2].split ("=")[0].split (" ");
        if variable [-1]=='':
            variable =variable [-2]
            
        
        else :
            variable =variable [-1]
            
        
        memory =memcrawl (gdb ,memory ,variable );
        
        #print("Malloc Found: "+variable)
        
    
    elif (re .search (r"^( |\t|\\t)*free\(.*\)",lines [-2])):
        variable =re .sub (r"^.*free\(","",lines [-2],count =1)
        variable =variable [0:-2]
        free_var (gdb ,memory ,variable );
        #print("Free found: "+variable)
        
    
    
    return (memory ,lines )
    


def render_memory_text (memory ):
    variable_render ="";
    mem_render =""
    for i in memory [0]:
        mem_render +=str (i )+": ("+str (memory [0][i ][0])+") "+str (memory [0][i ][1])+"\n"
        
    
    
    for i in memory [1]:
        variable_render +=str (i )+": "+str (memory [1][i ])+"\n"
        
    
    return (variable_render ,mem_render )
    


def main ():
    global memory 
    global lines 
    
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
    
    def handle_input (key ):
        global memory 
        global lines 
        if key in ["q","Q"]:
            raise urwid .ExitMainLoop ()
            
        elif key in ["n","N"]:
            gdb .writeln ("n");
            (memory ,lines )=update (gdb ,memory ,lines )
            (variable_render ,mem_render )=render_memory_text (memory )
            txt .set_text (mem_render )
            txt2 .set_text (variable_render )
            linestxt .set_text ("\n".join (lines [0:-1])+"\n\n"+lines [-1])
            
        
        
    
    
    
    
    txt =urwid .Text ("")
    mem =urwid .Filler (txt ,"top")
    
    txt2 =urwid .Text ("")
    var =urwid .Filler (txt2 ,"top")
    
    linestxt =urwid .Text ("")
    linesFiller =urwid .Filler (linestxt ,"bottom")
    
    pile =urwid .Pile ([mem ,var ,linesFiller ])
    
    top =urwid .Filler (pile ,valign ="top")
    loop =urwid .MainLoop (top ,unhandled_input =lambda key :handle_input (key ))
    loop .run ()
    
    


if (__name__ =="__main__"):
    main ();
    


