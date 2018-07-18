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

def GetMethodFromAddr(addr):
    return GetVarFromAddr(addr) & 0xFFFFFFFE
    
def IsCode(addr):
    return idc.isCode(idc.GetFlags(addr))

def IsData(addr):
    return idc.isData(idc.GetFlags(addr))

def IsSubFollowing(addr):
    i = 0
    while i < 20:
        pAddr = GetMethodFromAddr(addr)
        if not IsCode(pAddr):
            return False;

        addr = idc.NextHead(addr)
        i = i + 1

    return True

def IsDataFollowing(addr):
    i = 0
    while i < 20:
        if not IsData(addr):
            return False;

        addr = idc.NextHead(addr)
        i = i + 1

    return True

def LocateMethodPointers():
    seg = idc.FirstSeg()
    initArrayAddr = 0
    while seg != idc.BADADDR:
        seg = idc.NextSeg(seg)
        segName = idc.SegName(seg)
        if segName == ".data.rel.ro":
            data_rel_ro = idc.SegStart(seg)
            break

    addr = data_rel_ro
    referedVars = []
    while idc.SegName(addr) == ".data.rel.ro":
        for r in idautils.XrefsTo(addr,0):
            #print "is refered: 0x%x" % addr
            referedVars.append(addr)
            break

        addr += 4

    candidateMethodPointers = []
    for var in referedVars:
        if IsSubFollowing(var):
            candidateMethodPointers.append(var)

    for candidate in candidateMethodPointers:
        for referedVar in referedVars:
            if referedVar == candidate:
                nextVar = referedVars[referedVars.index(referedVar)+1]
                print "candidate: 0x%x, candidate end: 0x%x, method numbers: %d" % (candidate, nextVar, (nextVar-candidate)/4)
                break


def LocateStringLiterals():
    seg = idc.FirstSeg()
    initArrayAddr = 0
    while seg != idc.BADADDR:
        seg = idc.NextSeg(seg)
        segName = idc.SegName(seg)
        if segName == ".data.rel.ro":
            data_rel_ro = idc.SegStart(seg)
            break

    addr = data_rel_ro
    referedVars = []
    while idc.SegName(addr) == ".data.rel.ro":
        for r in idautils.XrefsTo(addr,0):
            referedVars.append(addr)
            break
        addr += 4

    candidateMetadaUsages = []
    for idx, var in enumerate(referedVars):
        if idx < (len(referedVars)-1) and (referedVars[idx+1]-referedVars[idx]) >= 1024:
            if idc.Dword(var) == 0x0:
                continue
            if IsDataFollowing(idc.Dword(var)) and idc.SegName(idc.Dword(var) ) == '.bss':
                candidateMetadaUsages.append(var)

    for candidate in candidateMetadaUsages:
        for referedVar in referedVars:
            if referedVar == candidate:
                nextVar = referedVars[referedVars.index(referedVar)+1]
                print "candidate: 0x%x, candidate end: 0x%x, method numbers: %d" % (candidate, nextVar, (nextVar-candidate)/4)
                break


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

print "Type LocateMethodPointers() to print suggested candidate for method pointers"
print "Click the location where you believe the method pointers start, type LoadMethods()"
print "Type LocateStringLiterals() to print suggested candidate for string literals"
print "Click the location where you believe the string literals start, type LoadStringLiterals()"
