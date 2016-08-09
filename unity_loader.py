import idaapi
import os
import re


def ParseString():

    ea = ScreenEA();
    
    path = os.getcwd()
    os.system(path+'/unity_decoder.exe')
    file = open('./string_literal.txt')
    
    str_count = file.readline()
    i = 0;

    for line in file:
        line = line.replace(' ', '_')
        line = re.sub(r'[^a-zA-Z0-9_]', '', line)
        
        if len(line) > 0 and line[0].isdigit():
            line = 's' + line
        
        addr = idc.Qword(ea)
        idc.MakeName(addr, str(line))
        
        ea = ea - 8
    
    file.close()
    

 
 
# IDA binds hotkeys to IDC functions so a trampoline IDC function
# must be created
idaapi.CompileLine('static ParseString() { RunPythonStatement("ParseString()"); }')
# Add the hotkey
AddHotkey("Alt-F8", 'ParseString')

