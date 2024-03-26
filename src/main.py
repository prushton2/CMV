#!/bin/bython
import pexpect

analyzer = pexpect.spawn('gdb', encoding='utf-8')
analyzer.expect(r'\(gdb\) ')


ipt = input("Enter executable file:\n> ")

analyzer.sendline("file "+ipt);
analyzer.expect(r'\(gdb\) ')

analyzer.sendline(f"b 1\n");
analyzer.expect(r'\(gdb\) ')

analyzer.sendline(f"r\ny\n");
analyzer.expect(r'\(gdb\) ')

while(ipt != "q") {
    
    #print("TRYING:: Trying", ipt, "...")
    analyzer.sendline(ipt)
    if(ipt == "r") {
        analyzer.sendline("y");
    }
    analyzer.expect(r"\(gdb\) ")
    print(analyzer.before)

    analyzer.sendline("info locals");
    analyzer.expect(r"\(gdb\) ")
    print(analyzer.before)
    
    ipt = input(">");
}
print("Done");
