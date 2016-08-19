import idaapi
import os
import re

def ParseMethod():
    
    ea = ScreenEA();
    
    path = os.getcwd()
    os.system(path+'/unity_decoder.exe')
    file = open('./method_name.txt')
    
    str_count = file.readline()
    i = 0;

    for line in file:
        line = line.replace(' ', '_')
        line = re.sub(r'[^a-zA-Z0-9_$]', '', line)
        
        print line
        if len(line) > 0 and line[0].isdigit():
            line = 's' + line
        
        #addr = idc.Qword(ea)
        #idc.MakeName(addr, str(line))
        idc.MakeComm(ea, str(line))
        
        #ea = ea + 8
        ea = ea + 4
    
    file.close()
    

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
    

 
# must be created
idaapi.CompileLine('static ParseString() { RunPythonStatement("ParseString()"); }')
idaapi.CompileLine('static ParseMethod() { RunPythonStatement("ParseMethod()"); }')

# Add the hotkey
AddHotkey("Alt-F8", 'ParseString')
AddHotkey("Alt-F9", 'ParseMethod')

