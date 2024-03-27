#!/bin/bython
import pexpect
import re

analyzer = pexpect.spawn('gdb', encoding='utf-8')
analyzer.expect(r"\(gdb\) ")
print(analyzer.before)

ipt = input("Enter executable file:\n> ")

analyzer.sendline("\nfile "+ipt);

analyzer.expect(r"\(gdb\) ")
analyzer.sendline(f"b 1\n");

analyzer.expect(r'\(gdb\) ')
analyzer.sendline(f"r\ny\n");


while(1) {
    ipt = input("> ");
    #print("TRYING:: Trying", ipt, "...")
    analyzer.expect(r"\(gdb\) ")
    analyzer.sendline(ipt)
    out = (analyzer.before)
    
   if re.search(r"[0-9]*\s*free\(.*\)", out) {
       print("Free statement present here");
   }

   print(out)

   #analyzer.sendline("info locals");
   #analyzer.expect(r"\(gdb\) ")
   ##print(analyzer.before)   
}
print("Done");
