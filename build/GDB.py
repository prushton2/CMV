import subprocess 
from mlib import *

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
        
    
    
