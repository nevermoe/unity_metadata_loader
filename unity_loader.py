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
        
        if len(line) > 0 and line[0].isdigit():
            line = 's' + line
        
        if bits == 32: 
            addr = idc.Dword(ea)
            idc.MakeNameEx(addr, str(line), SN_NOWARN)
            ea = ea + 4
            
        elif bits == 64:
            addr = idc.Qword(ea)
            idc.MakeNameEx(addr, str(line), SN_NOWARN)
            ea = ea + 8
        
    
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
        
        if bits == 64:
            addr = idc.Qword(ea)
            idc.MakeNameEx(addr, str(line), SN_NOWARN)
            ea = ea - 8
        elif bits == 32:
            print line
            addr = idc.Dword(ea)
            idc.MakeNameEx(addr, str(line), SN_NOWARN)
            ea = ea - 4
    
    file.close()
    

info = idaapi.get_inf_structure()
bits = None

if info.is_64bit():
    bits = 64
elif info.is_32bit():
    bits = 32
else:
    bits = 16    
    
 
# must be created
idaapi.CompileLine('static ParseString() { RunPythonStatement("ParseString()"); }')
idaapi.CompileLine('static ParseMethod() { RunPythonStatement("ParseMethod()"); }')

# Add the hotkey
AddHotkey("Alt-F8", 'ParseString')
AddHotkey("Alt-F9", 'ParseMethod')

