#!/bin/bython
import time 
import sys 
import re 

from GDB import GDB 
from mlib import *
import textual 

from textual .app import App ,ComposeResult 
from textual .widgets import Header ,Footer ,Button ,Label ,Static 
from textual .reactive import reactive ,var 


def update (gdb ,memory ,lines ):
    out =gdb .read ();
    
    if len (get_lines_by_prefix (out ,"^"))>0:
        if get_lines_by_prefix (out ,"^")[0].startswith ("^error"):
            return (memory ,lines ,out +"\n detected")
            
        
        
    
    
    out =gdb .read ()
    
    lines .append (get_lines_by_prefix (out ,"~")[0][2:-3])
    #remove preceeding integers from line
    lines [-1]=lines [-1][len (re .findall (r"^[0-9]*",lines [-1])[0]):]
    lines [-1]=re .sub (r"\\t","",lines [-1])
    
    
    memory =map_memory (gdb ,memory );
    
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
        
    
    
    return (memory ,lines ,out )
    


def render_memory_text (memory ):
    variable_render ="";
    mem_render =""
    for i in memory [0]:
        mem_render +=str (i )+": ("+str (memory [0][i ][0])+") "+str (memory [0][i ][1])+"\n"
        
    
    
    for i in memory [1]:
        variable_render +=str (i )+": "+str (memory [1][i ])+"\n"
        
    
    return (mem_render ,variable_render )
    



class TUI (App ):
    memory =(dict (),dict ())
    lines =['','','']
    gdb =None 
    
    def on_button_pressed (self ,event ):
        self .gdb .writeln ("n")
        (self .memory ,self .lines ,debug )=update (self .gdb ,self .memory ,self .lines );
        
        rendered_text =render_memory_text (self .memory )
        
        self .query_one ("#mem").update (rendered_text [0])
        self .query_one ("#vars").update (rendered_text [1])
        self .query_one ("#lines").update ("\n".join (self .lines [-4:-1]))
        self .query_one ("#debug").update (debug )
        
    
    
    def compose (self ):
        yield Header ();
        yield Button ("Next");
        yield Static (str (self .gdb ),id ="mem");
        yield Static ("",id ="vars");
        yield Static ("",id ="lines");
        yield Static ("",id ="debug");
        yield Footer ();
        
    
    



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
    
    tui =TUI ();
    tui .gdb =gdb ;
    tui .run ();
    
    


if (__name__ =="__main__"):
    main ();
    

''' 
def handle_input(key) {
        global memory
        global lines
        if key in ["q", "Q"] {
            raise urwid.ExitMainLoop()
        } elif key in ["n", "N"] {
            gdb.writeln("n");
            (memory, lines) = update(gdb, memory, lines)
            (variable_render, mem_render) = render_memory_text(memory)
            txt.set_text(mem_render)
            txt2.set_text(variable_render)
            linestxt.set_text("\n".join(lines[-3:-1]) + "\n\n" + lines[-1])
        }
    }


    memtxt = urwid.Text("", align="right")
    memFiller = urwid.Filler(txt, "top")
    
    vartxt = urwid.Text("", align="right")
    varFiller = urwid.Filler(txt2, "top")

    linestxt = urwid.Text("")
    linesFiller = urwid.Filler(linestxt, "bottom")

    pile = urwid.Pile([mem, var, linesFiller])

    top = urwid.Filler(pile, valign="top")
    loop = urwid.MainLoop(top, unhandled_input=lambda key: handle_input(key))
    loop.run()
'''
