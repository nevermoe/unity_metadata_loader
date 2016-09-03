import idaapi
import os, sys
import re
import random


def IncreaseAddr(addr):
    if BITS == 64:
        return addr+8
    elif BITS == 32:
        return addr+4
        
        
def DecreaseAddr(addr):
    if BITS == 64:
        return addr-8
    elif BITS == 32:
        return addr-4
        
def GetVarFromAddr(addr):
    if BITS == 64:
        return idc.Qword(addr)
    elif BITS == 32:
        return idc.Dword(addr)

def LoadMethods(ea = None):
    
    if ea is None:
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
        addr = GetVarFromAddr(ea)
        ret = idc.MakeNameEx(addr, str(new_line), SN_NOWARN)
        while ret == 0 and i < 5: # failed
            new_line_rand = new_line + '_' + str(random.randint(0, 99999))
            ret = idc.MakeNameEx(addr, str(new_line_rand), SN_NOWARN)
            idc.MakeComm(ea, str(line))
            i = i + 1
            
        ea = IncreaseAddr(ea)

        
    
    file.close()
    

def LoadStrings(ea = None):

    if ea is None:
        ea = ScreenEA();
    
    path = os.getcwd()
    os.system(path+'/unity_decoder.exe')
    file = open('./string_literal.txt')
    
    str_count = file.readline()

    for line in file:
        line = line.strip(' ').replace('\r', '').replace('\n', '')
        new_line = re.sub(r'[^a-zA-Z0-9_]', '_', line)
        new_line = 'StringLiteral_' + new_line
        
       
        i = 0;
        addr = GetVarFromAddr(ea)
        ret = idc.MakeNameEx(addr, str(new_line), SN_NOWARN)
        while ret == 0 and i < 5: # failed
            new_line_rand = new_line + '_' + str(random.randint(0, 99999))
            ret = idc.MakeNameEx(addr, str(new_line_rand), SN_NOWARN)
            i = i + 1
             
        idc.MakeComm(ea, str(line))
        ea = DecreaseAddr(ea)
            

    
    file.close()

    
def IsCode(addr):
    return idc.isCode(idc.GetFlags(addr))
    
def IsData(addr):
    return idc.isData(idc.GetFlags(addr))
    
def IsSubFollowing(addr):
    i = 0
    while i < 20:
        pAddr = GetVarFromAddr(addr)
        if not IsCode(pAddr):
            return False;
        
        addr = idc.NextHead(addr)
        i = i + 1
        
    return True
    
    
def IsStringPreceding(addr):
    i = 0
    while i < 10:
        pAddr = GetVarFromAddr(addr)
        if not IsData(pAddr):
            return False;
        
        addr = idc.PrevHead(addr)
        i = i + 1
        
    return True
    
def IsStringFollowing(addr):
    i = 0
    while i < 10:
        #pAddr = GetVarFromAddr(addr)
        #if not IsData(pAddr):
        if not IsData(addr):
            return False;
        
        addr = idc.NextHead(addr)
        i = i + 1
        
    return True

    
def AutoLoadAndroid():
    constSegList = [];
    seg = idc.FirstSeg()
    while seg != idc.BADADDR:
        seg = idc.NextSeg(seg)
        segName = idc.SegName(seg)
        if segName == ".data.rel.ro":
            constSegList.append(seg)
    
    print "Locating Methods and Strings..."
    for addr in constSegList:
        while idc.SegName(addr) == '.data.rel.ro':
            if IsSubFollowing(addr) and IsStringFollowing(GetVarFromAddr(idc.PrevHead(addr))):
                print "Location Found At: %x" % (addr)
                idc.Jump(addr)
                break;
            
            addr = idc.NextHead(addr)
    
    print "Loading Symbols..."
    LoadMethods(addr)
    #LoadStrings(idc.PrevHead(addr))
    
    print "Done."
    
def AutoLoadiOS():
    constSegList = [];
    seg = idc.FirstSeg()
    while seg != idc.BADADDR:
        seg = idc.NextSeg(seg)
        segName = idc.SegName(seg)
        if segName == "__const":
            constSegList.append(seg)
    
    print "Locating Methods and Strings..."
    for addr in constSegList:
        while idc.SegName(addr) == '__const':
            if IsSubFollowing(addr) and IsStringPreceding(idc.PrevHead(addr)):
                print "Location Found At: %x" % (addr)
                idc.Jump(addr)
                break;
            
            addr = idc.NextHead(addr)
    
    print "Loading Symbols..."
    LoadMethods(addr)
    LoadStrings(idc.PrevHead(addr))
    
    print "Done."
    
    
def AutoLoad():

    if FILE_TYPE == "f_MACHO":
        AutoLoadiOS()
    elif FILE_TYPE == "f_ELF":
        AutoLoadAndroid()
    else:
        print "AutoLoad is not supported for your platform. Please try LocationHelper."
    
    
def LocationHelper():
    return True;
    
    
ENUM_FILE_TYPE =\
[
  "f_EXE_old",            #// MS DOS EXE File
  "f_COM_old",            #// MS DOS COM File
  "f_BIN",                #// Binary File
  "f_DRV",                #// MS DOS Driver
  "f_WIN",                #// New Executable (NE)
  "f_HEX",                #// Intel Hex Object File
  "f_MEX",                #// MOS Technology Hex Object File
  "f_LX",                 #// Linear Executable (LX)
  "f_LE",                 #// Linear Executable (LE)
  "f_NLM",                #// Netware Loadable Module (NLM)
  "f_COFF",               #// Common Object File Format (COFF)
  "f_PE",                 #// Portable Executable (PE)
  "f_OMF",                #// Object Module Format
  "f_SREC",               #// R-records
  "f_ZIP",                #// ZIP file (this file is never loaded to IDA database)
  "f_OMFLIB",             #// Library of OMF Modules
  "f_AR",                 #// ar library
  "f_LOADER",             #// file is loaded using LOADER DLL
  "f_ELF",                #// Executable and Linkable Format (ELF)
  "f_W32RUN",             #// Watcom DOS32 Extender (W32RUN)
  "f_AOUT",               #// Linux a.out (AOUT)
  "f_PRC",                #// PalmPilot program file
  "f_EXE",                #// MS DOS EXE File
  "f_COM",                #// MS DOS COM File
  "f_AIXAR",              #// AIX ar library
  "f_MACHO",              #// Max OS X
];

info = idaapi.get_inf_structure()
BITS = None


if info.is_64bit():
    BITS = 64
elif info.is_32bit():
    BITS = 32
else:
    BITS = 16    

FILE_TYPE = ENUM_FILE_TYPE[info.filetype]
#print FILE_TYPE
 
# must be created
idaapi.CompileLine('static LoadStrings() { RunPythonStatement("LoadStrings()"); }')
idaapi.CompileLine('static LoadMethods() { RunPythonStatement("LoadMethods()"); }')
idaapi.CompileLine('static AutoLoad() { RunPythonStatement("AutoLoad()"); }')
idaapi.CompileLine('static LocationHelper() { RunPythonStatement("LocationHelper()"); }')

# Add the hotkey
AddHotkey("Ctrl-Alt-S", 'LoadStrings')
AddHotkey("Ctrl-Alt-M", 'LoadMethods')
AddHotkey("Ctrl-Alt-A", 'AutoLoad')
AddHotkey("Ctrl-Alt-L", 'LocationHelper')

print "Ctrl-Alt-S : Load String Literals"
print "Ctrl-Alt-M : Load Methods"
print "Ctrl-Alt-A : Automatically Load Everything (iOS only)"
print "Ctrl-Alt-L : Locate Methods (Experimental)"


