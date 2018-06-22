import os, sys
import re
import random
import idaapi
import idc
import idautils


INFO = idaapi.get_inf_structure()

if INFO.is_64bit():
    BITS = 64
elif INFO.is_32bit():
    BITS = 32
else:
    BITS = 16


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


FILE_TYPE = ENUM_FILE_TYPE[INFO.filetype]


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

def LocatePointerLists():
    seg = idc.FirstSeg()
    initArrayAddr = 0
    while seg != idc.BADADDR:
        seg = idc.NextSeg(seg)
        segName = idc.SegName(seg)
        if segName == ".init_array":
            initArrayAddr = idc.SegStart(seg)
            break
    
    # find Il2CppCodeRegistrationOffset from init_array
    Il2CppCodeRegistrationOffset = initArrayAddr + 30 * (BITS / 8)
    print "find Il2CppCodeRegistrationOffset %x" % Il2CppCodeRegistrationOffset

    Il2CppCodeRegistrationCpp = GetVarFromAddr(Il2CppCodeRegistrationOffset)

    print "Il2CppCodeRegistrationCpp: %x" % Il2CppCodeRegistrationCpp
    idc.MakeName(Il2CppCodeRegistrationCpp, "Il2CppCodeRegistrationCpp")
    
    #Il2CppCodegenRegistration = 0
    #for r in idautils.XrefsFrom(Il2CppCodeRegistrationAddr + 0x14, 0):
    #    Il2CppCodegenRegistration = hex(r.to)
        
    #g_CodeRegistration = 0
    #for r in idautils.XrefsFrom(Il2CppCodegenRegistration + 0x18, 0):
    #    g_CodeRegistration = hex(r.to)
    
    opndValue = idc.GetOperandValue(Il2CppCodeRegistrationCpp + 0x8,1)
    offset = GetVarFromAddr(opndValue)
    
    _GLOBAL_OFFSET_TABLE_ = idc.LocByName("_GLOBAL_OFFSET_TABLE_")
    
    
    print "_GLOBAL_OFFSET_TABLE_ %x" % _GLOBAL_OFFSET_TABLE_
    
    
    Il2CppCodegenRegistration = (_GLOBAL_OFFSET_TABLE_ + offset) & 0xFFFFFFFF
    idc.MakeName(Il2CppCodegenRegistration, "Il2CppCodegenRegistration")
    print "Il2CppCodegenRegistration %x" % Il2CppCodegenRegistration
    
    opndValue = idc.GetOperandValue(Il2CppCodegenRegistration + 0xC,1)
    offset = GetVarFromAddr(opndValue)
    g_CodeRegistration = (_GLOBAL_OFFSET_TABLE_ + offset) & 0xFFFFFFFF
    idc.MakeName(g_CodeRegistration, "g_CodeRegistration")
    print "g_CodeRegistration %x" % g_CodeRegistration
    
    g_MethodPointers = GetVarFromAddr(g_CodeRegistration + 0x4)
    idc.MakeName(g_MethodPointers, "g_MethodPointers")
    print "g_MethodPointers %x" % g_MethodPointers
    
    
    opndValue = idc.GetOperandValue(Il2CppCodegenRegistration + 0x04,1)
    offset = GetVarFromAddr(opndValue)
    g_MetadataRegistration = GetVarFromAddr((_GLOBAL_OFFSET_TABLE_ + offset) & 0xFFFFFFFF)
    idc.MakeName(g_MetadataRegistration, "g_MetadataRegistration")
    print "g_MetadataRegistration %x" % g_MetadataRegistration
    
    g_MetadataUsages = GetVarFromAddr(g_MetadataRegistration + 0x3C)
    idc.MakeName(g_MetadataUsages, "g_MetadataUsages")
    print "g_MetadataUsages %x" % g_MetadataUsages
    
    return (g_MethodPointers, g_MetadataUsages)

def GetMethodFromAddr(addr):
    return GetVarFromAddr(addr) & 0xFFFFFFFE
    
    
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
        addr = GetMethodFromAddr(ea)
        ret = idc.MakeNameEx(addr, str(new_line), SN_NOWARN)
        while ret == 0 and i < 5: # failed
            new_line_rand = new_line + '_' + str(random.randint(0, 99999))
            ret = idc.MakeNameEx(addr, str(new_line_rand), SN_NOWARN)
            idc.MakeComm(ea, str(line))
            i = i + 1
            
        ea = IncreaseAddr(ea)

    file.close()
    
def LoadStringLiterals(ea = None):

    if ea is None:
        ea = ScreenEA();
    
    path = os.getcwd()
    os.system(path+'/unity_decoder.exe')
    file = open('./string_literal.txt')
    
    str_count = file.readline()
    skip_count = file.readline()
    ea += int(skip_count) * 0x4

    for line in file:
        line = line.strip(' ').replace('\r', '').replace('\n', '')
        new_line = re.sub(r'[^a-zA-Z0-9_]', '_', line)
        new_line = 'StringLiteral_' + new_line
        
       
        i = 0;
        addr = GetVarFromAddr(ea)
        ret = idc.MakeNameEx(addr, str(new_line), SN_NOWARN)
        #ret = idc.MakeNameEx(addr, "", SN_NOWARN)
        while ret == 0 and i < 5: # failed
            new_line_rand = new_line + '_' + str(random.randint(0, 99999))
            ret = idc.MakeNameEx(addr, str(new_line_rand), SN_NOWARN)
            i = i + 1
             
        idc.MakeComm(ea, str(line))
        #idc.MakeComm(ea, "")
        ea = IncreaseAddr(ea)
    
    file.close()


def AutoLoadAndroid():
    (g_MethodPointers,g_MetadataUsages) = LocatePointerLists()
    LoadMethods(g_MethodPointers)
    LoadStringLiterals(g_MetadataUsages)

def AutoLoad():

    if FILE_TYPE == "f_ELF":
        AutoLoadAndroid()
    else:
        print "AutoLoad is not supported for your platform. Please try LocationHelper."

# must be created
idaapi.CompileLine('static LoadStringLiterals() { RunPythonStatement("LoadStringLiterals()"); }')
idaapi.CompileLine('static LoadMethods() { RunPythonStatement("LoadMethods()"); }')
idaapi.CompileLine('static AutoLoad() { RunPythonStatement("AutoLoad()"); }')
idaapi.CompileLine('static LocatePointerLists() { RunPythonStatement("LocatePointerLists()"); }')

# Add the hotkey
AddHotkey("Ctrl-Alt-S", 'LoadStringLiterals')
AddHotkey("Ctrl-Alt-M", 'LoadMethods')
AddHotkey("Ctrl-Alt-A", 'AutoLoad')
AddHotkey("Ctrl-Alt-L", 'LocatePointerLists')

print "Ctrl-Alt-A : Automatically Load Everything"
print "Ctrl-Alt-S : (Advanced) Load String Literals"
print "Ctrl-Alt-M : (Advanced) Load Methods"
print "Ctrl-Alt-L : Locate Method Pointers and Stringliteral Pointers"
