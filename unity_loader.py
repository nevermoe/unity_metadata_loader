import idaapi
import os
import re
import random

def ParseMethod():
    
    ea = ScreenEA();
    
    path = os.getcwd()
    os.system(path+'/unity_decoder.exe')
    file = open('./method_name.txt')
    
    str_count = file.readline()
    i = 0;

    for line in file:
        line = line.strip(' ').replace('\r', '').replace('\n', '')
        new_line = re.sub(r'[^a-zA-Z0-9_$]', '_', line)
        
        i = 0;
        if bits == 32: 
            addr = idc.Dword(ea)
            ret = idc.MakeNameEx(addr, str(new_line), SN_NOWARN)
            while ret == 0 and i < 5: # failed
                new_line_rand = new_line + '_' + str(random.randint(0, 99999))
                ret = idc.MakeNameEx(addr, str(new_line_rand), SN_NOWARN)
                idc.MakeComm(ea, str(line))
                i = i + 1
                
            ea = ea + 4
            
        elif bits == 64:
            addr = idc.Qword(ea)
            ret = idc.MakeNameEx(addr, str(new_line), SN_NOWARN)
            while ret == 0 and i < 5: # failed
                new_line_rand = new_line + '_' + str(random.randint(0, 99999))
                ret = idc.MakeNameEx(addr, str(new_line_rand), SN_NOWARN)
                idc.MakeComm(ea, str(line))
                i = i + 1
                
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
        line = line.strip(' ').replace('\r', '').replace('\n', '')
        new_line = re.sub(r'[^a-zA-Z0-9_]', '_', line)
        
        if len(new_line) > 0 and new_line[0].isdigit():
            new_line = 'StringLiteral_' + new_line
        
        i = 0;
        if bits == 64:
            addr = idc.Qword(ea)
            ret = idc.MakeNameEx(addr, str(new_line), SN_NOWARN)
            while ret == 0 and i < 5: # failed
                new_line_rand = 'StringLiteral' + new_line + '_' + str(random.randint(0, 99999))
                ret = idc.MakeNameEx(addr, str(new_line_rand), SN_NOWARN)
                i = i + 1
                
            idc.MakeComm(ea, str(line))
            ea = ea - 8
            
        elif bits == 32:
            addr = idc.Dword(ea)
            ret = idc.MakeNameEx(addr, str(new_line), SN_NOWARN)
            while ret == 0 and i < 5: # failed
                new_line_rand = 'StringLiteral_' + new_line + '_' + str(random.randint(0, 99999))
                ret = idc.MakeNameEx(addr, str(new_line_rand), SN_NOWARN)
                i = i + 1
                
            idc.MakeComm(ea, str(line))
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

