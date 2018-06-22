#include "il2cpp-config.h"

#include "icalls/mscorlib/System.Runtime.InteropServices/Marshal.h"

#include "class-internals.h"
#include "tabledefs.h"

#include "gc/GarbageCollector.h"
#include "metadata/FieldLayout.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Field.h"
#include "vm/Image.h"
#include "vm/LastError.h"
#include "vm/MarshalAlloc.h"
#include "vm/Object.h"
#include "vm/PlatformInvoke.h"
#include "vm/RCW.h"
#include "vm/String.h"
#include "vm/Type.h"
#include "utils/MarshalingUtils.h"
#include "utils/StringUtils.h"
#include <sstream>
#include <string>
#include <sstream>
#include <deque>

using namespace il2cpp::vm;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Runtime
{
namespace InteropServices
{
    int32_t Marshal::AddRefInternal(Il2CppIntPtr pUnk)
    {
        return static_cast<Il2CppIUnknown*>(pUnk.m_value)->AddRef();
    }

    Il2CppIntPtr Marshal::AllocCoTaskMem(int32_t size)
    {
        Il2CppIntPtr result;
        result.m_value = MarshalAlloc::Allocate(size);
        return result;
    }

    Il2CppIntPtr Marshal::AllocHGlobal(Il2CppIntPtr size)
    {
        Il2CppIntPtr result;
        result.m_value = MarshalAlloc::AllocateHGlobal((size_t)size.m_value);
        return result;
    }

    void Marshal::copy_from_unmanaged(Il2CppIntPtr source, int startIndex, Il2CppArray * destination, int length)
    {
        uint32_t element_size = (uint32_t)il2cpp_array_element_size(destination->klass);
        memcpy(il2cpp_array_addr_with_size(destination, element_size, startIndex), source.m_value, length * element_size);
    }

    void Marshal::copy_to_unmanaged(Il2CppArray * source, int32_t startIndex, Il2CppIntPtr destination, int32_t length)
    {
        uint32_t element_size = (uint32_t)il2cpp_array_element_size(source->klass);
        memcpy(destination.m_value, il2cpp_array_addr_with_size(source, element_size, startIndex), length * element_size);
    }

    void Marshal::FreeBSTR(Il2CppIntPtr ptr)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::StringToBSTR, "BSTR icalls are not supported.");
    }

    void Marshal::FreeCoTaskMem(Il2CppIntPtr ptr)
    {
        MarshalAlloc::Free(ptr.m_value);
    }

    void Marshal::FreeHGlobal(Il2CppIntPtr hglobal)
    {
        MarshalAlloc::FreeHGlobal(hglobal.m_value);
    }

    bool Marshal::IsComObject(Il2CppObject* o)
    {
        return o->klass->is_import_or_windows_runtime;
    }

    Il2CppIntPtr Marshal::GetCCW(Il2CppObject* o, Il2CppReflectionType * T)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::GetCCW, "COM icalls are not supported.");
        return Il2CppIntPtr::Zero;
    }

    int32_t Marshal::GetComSlotForMethodInfoInternal(mscorlib_System_Reflection_MemberInfo * m)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::GetComSlotForMethodInfoInternal, "COM icalls are not supported.");
        return 0;
    }

    Il2CppDelegate* Marshal::GetDelegateForFunctionPointerInternal(Il2CppIntPtr ptr, Il2CppReflectionType* t)
    {
        Il2CppClass* delegateType = il2cpp::vm::Class::FromIl2CppType(t->type);
        return PlatformInvoke::MarshalFunctionPointerToDelegate(ptr.m_value, delegateType);
    }

    Il2CppIntPtr Marshal::GetFunctionPointerForDelegateInternal(Il2CppDelegate* d)
    {
        return vm::PlatformInvoke::MarshalDelegate(d);
    }

    Il2CppIntPtr Marshal::GetIDispatchForObjectInternal(Il2CppObject* o)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::GetIDispatchForObjectInternal, "COM icalls are not supported.");
        return Il2CppIntPtr::Zero;
    }

    Il2CppIntPtr Marshal::GetIUnknownForObjectInternal(Il2CppObject* o)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::GetIUnknownForObjectInternal, "This icall is not supported by il2cpp. Use the il2cpp_codegen_com_get_iunknown_for_object intrinsic instead.");
        return Il2CppIntPtr::Zero;
    }

    Il2CppObject* Marshal::GetObjectForCCW(Il2CppIntPtr pUnk)
    {
        if (pUnk.m_value == NULL)
            return NULL;

        return RCW::GetOrCreateFromIUnknown(static_cast<Il2CppIUnknown*>(pUnk.m_value), il2cpp_defaults.il2cpp_com_object_class);
    }

    Il2CppString* Marshal::PtrToStringBSTR(Il2CppIntPtr ptr)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::PtrToStringBSTR, "COM icalls are not supported.");
        return 0;
    }

    Il2CppString* Marshal::PtrToStringAnsi_mscorlib_System_String_mscorlib_System_IntPtr(Il2CppIntPtr ptr)
    {
        char* value = (char*)ptr.m_value;
        if (value == NULL)
            return NULL;
        return String::New(value);
    }

    Il2CppString* Marshal::PtrToStringAnsi_mscorlib_System_String_mscorlib_System_IntPtr_mscorlib_System_Int32(Il2CppIntPtr ptr, int32_t len)
    {
        char* value = (char*)ptr.m_value;
        if (value == NULL)
            Exception::Raise(Exception::GetArgumentNullException("ptr"));

        return String::NewLen(value, len);
    }

    Il2CppString* Marshal::PtrToStringUni_mscorlib_System_String_mscorlib_System_IntPtr(Il2CppIntPtr ptr)
    {
        Il2CppChar* value = static_cast<Il2CppChar*>(ptr.m_value);
        if (value == NULL)
            return NULL;

        int32_t len = 0;
        Il2CppChar* t = value;

        while (*t++)
            len++;

        return String::NewUtf16(value, len);
    }

    Il2CppString* Marshal::PtrToStringUni_mscorlib_System_String_mscorlib_System_IntPtr_mscorlib_System_Int32(Il2CppIntPtr ptr, int32_t len)
    {
        Il2CppChar* value = static_cast<Il2CppChar*>(ptr.m_value);
        if (value == NULL)
            Exception::Raise(Exception::GetArgumentNullException("ptr"));

        return String::NewUtf16(value, len);
    }

    Il2CppObject* Marshal::PtrToStructure(Il2CppIntPtr ptr, Il2CppReflectionType* structureType)
    {
        if (ptr.m_value == NULL)
            return NULL;

        if (structureType == NULL)
            Exception::Raise(Exception::GetArgumentNullException("structureType"));

        Il2CppClass* type = Class::FromIl2CppType(structureType->type);

        Il2CppTypeEnum typeType = structureType->type->type;

        if (typeType == IL2CPP_TYPE_STRING || typeType == IL2CPP_TYPE_SZARRAY || (typeType == IL2CPP_TYPE_CLASS && !Class::HasDefaultConstructor(type)))
        {
            Exception::Raise(Exception::GetMissingMethodException("No parameterless constructor defined for this object."));
        }

        if (type->interopData != NULL && type->interopData->pinvokeMarshalFromNativeFunction != NULL)
        {
            Il2CppObject* result = Object::New(type);

            if (typeType == IL2CPP_TYPE_CLASS)
            {
                typedef void (*Constructor)(Il2CppObject*);
                Constructor ctor = reinterpret_cast<Constructor>(Class::GetMethodFromName(type, ".ctor", 0)->methodPointer);
                ctor(result);
                utils::MarshalingUtils::MarshalStructFromNative(ptr.m_value, result, type->interopData);
            }
            else
            {
                utils::MarshalingUtils::MarshalStructFromNative(ptr.m_value, Object::Unbox(result), type->interopData);
            }

            return result;
        }

        // If there's no custom marshal function, it means it's either a primitive, or invalid argument
        if (type->native_size != -1)
        {
            // We may also need to throw a NotSupportedException for an ArgIterator.
            if (structureType->type->type == IL2CPP_TYPE_VOID)
                Exception::Raise(Exception::GetNotSupportedException("Cannot dynamically create an instance of System.Void."));

            // PtrToStructure is supposed to throw on enums
            if (!type->enumtype)
            {
                Il2CppObject* result = Object::New(type);
                memcpy(Object::Unbox(result), ptr.m_value, type->native_size);
                return result;
            }
        }

        // If we got this far, throw an exception

        if (type->generic_class != NULL || type->is_generic)
            Exception::Raise(Exception::GetArgumentException("structure", "The specified object must not be an instance of a generic type."));

        Exception::Raise(Exception::GetArgumentException("structure", "The specified structure must be blittable or have layout information."));
        return NULL;
    }

    void Marshal::PtrToStructureObject(Il2CppIntPtr ptr, Il2CppObject* structure)
    {
        if (ptr.m_value == NULL)
            return;

        if (structure == NULL)
            Exception::Raise(Exception::GetArgumentNullException("structure"));

        Il2CppClass* type = structure->klass;

        // This is only legal for classes.
        if (type->byval_arg->type != IL2CPP_TYPE_CLASS)
        {
            Exception::Raise(Exception::GetArgumentException("structure", "The specified structure must be an instance of a formattable class."));
        }

        if (type->interopData != NULL && type->interopData->pinvokeMarshalFromNativeFunction != NULL)
        {
            utils::MarshalingUtils::MarshalStructFromNative(ptr.m_value, structure, type->interopData);
            return;
        }

        if (type->generic_class || type->is_generic)
            Exception::Raise(Exception::GetArgumentException("structure", "The specified object must not be an instance of a generic type."));

        Exception::Raise(Exception::GetArgumentException("structure", "The specified structure must be blittable or have layout information."));
    }

    int32_t Marshal::QueryInterfaceInternal(Il2CppIntPtr pUnk, mscorlib_System_Guid * iid, Il2CppIntPtr* ppv)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::QueryInterfaceInternal, "COM icalls are not supported.");
        return 0;
    }

    template<typename T>
    static inline T ReadValue(Il2CppIntPtr ptr, int32_t offset)
    {
        return *reinterpret_cast<T*>(static_cast<uint8_t*>(ptr.m_value) + offset);
    }

    uint8_t Marshal::ReadByte(Il2CppIntPtr ptr, int32_t ofs)
    {
        return ReadValue<uint8_t>(ptr, ofs);
    }

    int16_t Marshal::ReadInt16(Il2CppIntPtr ptr, int32_t ofs)
    {
        return ReadValue<int16_t>(ptr, ofs);
    }

    int32_t Marshal::ReadInt32(Il2CppIntPtr ptr, int32_t ofs)
    {
        return ReadValue<int32_t>(ptr, ofs);
    }

    int64_t Marshal::ReadInt64(Il2CppIntPtr ptr, int32_t ofs)
    {
        return ReadValue<int64_t>(ptr, ofs);
    }

    Il2CppIntPtr Marshal::ReadIntPtr(Il2CppIntPtr ptr, int32_t ofs)
    {
        return ReadValue<Il2CppIntPtr>(ptr, ofs);
    }

    int32_t Marshal::ReleaseInternal(Il2CppIntPtr pUnk)
    {
        return static_cast<Il2CppIUnknown*>(pUnk.m_value)->Release();
    }

    int32_t Marshal::ReleaseComObjectInternal(Il2CppObject* co)
    {
        // There's a check in mscorlib before calling this internal icall, so assert instead of full check is OK here.
        IL2CPP_ASSERT(co->klass->is_import_or_windows_runtime);

        // We can't really release the COM object directly, because it might have additional
        // fields that cache different interfaces. So let's just call its finalizer here.
        // In order to deal with the fact that this may get called from different threads
        // at the same time, we (atomically) register a NULL finalizer, and if another finalizer
        // was already registered, we call it. If there was no finalizer registered, it means
        // that we lost the race and we should just carry on.
        gc::GarbageCollector::FinalizerCallback oldFinalizer = gc::GarbageCollector::RegisterFinalizerWithCallback(co, NULL);
        if (oldFinalizer != NULL)
            oldFinalizer(co, NULL);

        return 0;
    }

    int Marshal::SizeOf(Il2CppReflectionType* rtype)
    {
        if (rtype == NULL)
            Exception::Raise(Exception::GetArgumentNullException("t"));

        Il2CppClass* typeInfo = Class::FromIl2CppType(rtype->type);

        if (typeInfo->native_size != -1)
        {
            // SizeOf "cannot" get size of enums and strings
            if (!typeInfo->enumtype && rtype->type->type != IL2CPP_TYPE_STRING)
                return typeInfo->native_size;
        }

        // If we get to here, type does not have a native size
        // So let's figure what kind of exception to throw

        if (typeInfo->generic_class != NULL || typeInfo->is_generic)
            Exception::Raise(Exception::GetArgumentException("t", "The t parameter is a generic type."));

        std::string exceptionMessage = utils::StringUtils::Printf("Type \'%s\' cannot be marshaled as an unmanaged structure; no meaningful size or offset can be computed.",
                Type::GetName(rtype->type, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME).c_str());

        Exception::Raise(Exception::GetArgumentException(NULL, exceptionMessage.c_str()));
        return 0;
    }

    Il2CppIntPtr Marshal::StringToBSTR(Il2CppString* s)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::StringToBSTR, "BSTR icalls are not supported.");
        return Il2CppIntPtr::Zero;
    }

    Il2CppIntPtr Marshal::StringToHGlobalUni(Il2CppString* s)
    {
        if (s == NULL)
            return Il2CppIntPtr::Zero;

        int32_t size = utils::StringUtils::GetLength(s);
        const Il2CppChar* utf16 = utils::StringUtils::GetChars(s);
        size_t bytes = (size + 1) * 2;
        Il2CppChar* cstr = static_cast<Il2CppChar*>(MarshalAlloc::AllocateHGlobal(bytes));
        memcpy(cstr, utf16, bytes);
        Il2CppIntPtr result;
        result.m_value = cstr;
        return result;
    }

    Il2CppIntPtr Marshal::StringToHGlobalAnsi(Il2CppString* s)
    {
        if (s == NULL)
            return Il2CppIntPtr::Zero;

        const Il2CppChar* utf16 = utils::StringUtils::GetChars(s);
        std::string str = il2cpp::utils::StringUtils::Utf16ToUtf8(utf16);
        char *cstr = (char*)MarshalAlloc::AllocateHGlobal(str.size() + 1);
        strcpy(cstr, str.c_str());
        Il2CppIntPtr result;
        result.m_value = cstr;
        return result;
    }

    void Marshal::StructureToPtr(Il2CppObject* structure, Il2CppIntPtr ptr, bool deleteOld)
    {
        if (structure == NULL)
            Exception::Raise(Exception::GetArgumentNullException("structure"));

        if (ptr.m_value == NULL)
            Exception::Raise(Exception::GetArgumentNullException("ptr"));

        Il2CppClass* type = structure->klass;

        if (type->interopData != NULL && type->interopData->pinvokeMarshalToNativeFunction != NULL)
        {
            if (deleteOld)
                utils::MarshalingUtils::MarshalFreeStruct(ptr.m_value, type->interopData);

            void* objectPtr = (type->byval_arg->type == IL2CPP_TYPE_CLASS) ? structure : Object::Unbox(structure);
            utils::MarshalingUtils::MarshalStructToNative(objectPtr, ptr.m_value, type->interopData);
            return;
        }

        // If there's no custom marshal function, it means it's either a primitive, or invalid argument

        if (type->native_size != -1)
        {
            // StructureToPtr is supposed to throw on strings and enums
            if (!type->enumtype && type->byval_arg->type != IL2CPP_TYPE_STRING)
            {
                memcpy(ptr.m_value, Object::Unbox(structure), type->native_size);
                return;
            }
        }

        // If we got this far, throw an exception
        Il2CppException* exception;

        if (type->generic_class != NULL || type->is_generic)
        {
            exception = Exception::GetArgumentException("structure", "The specified object must not be an instance of a generic type.");
        }
        else
        {
            exception = Exception::GetArgumentException("structure", "The specified structure must be blittable or have layout information.");
        }

        Exception::Raise(exception);
    }

    template<typename T>
    static inline void WriteValue(Il2CppIntPtr ptr, int32_t offset, T value)
    {
        *reinterpret_cast<T*>(static_cast<uint8_t*>(ptr.m_value) + offset) = value;
    }

    void Marshal::WriteByte(Il2CppIntPtr ptr, int32_t ofs, uint8_t val)
    {
        WriteValue(ptr, ofs, val);
    }

    void Marshal::WriteInt16(Il2CppIntPtr ptr, int32_t ofs, int16_t val)
    {
        WriteValue(ptr, ofs, val);
    }

    void Marshal::WriteInt32(Il2CppIntPtr ptr, int32_t ofs, int32_t val)
    {
        WriteValue(ptr, ofs, val);
    }

    void Marshal::WriteInt64(Il2CppIntPtr ptr, int32_t ofs, int64_t val)
    {
        WriteValue(ptr, ofs, val);
    }

    void Marshal::WriteIntPtr(Il2CppIntPtr ptr, int32_t ofs, Il2CppIntPtr val)
    {
        WriteValue(ptr, ofs, val);
    }

    void Marshal::DestroyStructure(Il2CppIntPtr ptr, Il2CppReflectionType* structureType)
    {
        if (ptr.m_value == NULL)
            Exception::Raise(Exception::GetArgumentNullException("ptr"));

        if (structureType == NULL)
            Exception::Raise(Exception::GetArgumentNullException("structureType"));

        Il2CppClass* type = Class::FromIl2CppType(structureType->type);

        // If cleanup function exists, it will call it and return true
        // In that case, we're done.
        if (utils::MarshalingUtils::MarshalFreeStruct(ptr.m_value, type->interopData))
            return;

        if (type->is_generic)
        {
            Exception::Raise(Exception::GetArgumentException("structureType", "The specified type must not be an instance of a generic type."));
        }

        // Enums are blittable, but they don't have layout information, therefore Marshal.DestroyStructure is supposed to throw
        if (!type->enumtype)
        {
            // Char and Boolean are not blittable, but they should not raise this exception, as we can call DestroyStructure on them without problems.
            if (type->is_blittable || structureType->type->type == IL2CPP_TYPE_CHAR || structureType->type->type == IL2CPP_TYPE_BOOLEAN)
                return;
        }

        // If we got this far, throw an exception
        Exception::Raise(Exception::GetArgumentException("structureType", "The specified type must be blittable or have layout information."));
    }

    int32_t Marshal::GetLastWin32Error()
    {
        return vm::LastError::GetLastError();
    }

    static size_t RoundUpToMultiple(size_t numToRound, size_t multiple)
    {
        if (multiple == 0)
            return numToRound;

        size_t remainder = numToRound % multiple;
        if (remainder == 0)
            return numToRound;
        return numToRound + multiple - remainder;
    }

    Il2CppIntPtr Marshal::OffsetOf(Il2CppReflectionType* t, Il2CppString* fieldName)
    {
        std::string fieldNameToFind = utils::StringUtils::Utf16ToUtf8(fieldName->chars);
        Il2CppClass* type = Class::FromIl2CppType(t->type);

        FieldInfo* field = vm::Class::GetFieldFromName(type, fieldNameToFind.c_str());
        if (field == NULL || (vm::Field::GetFlags(field) & FIELD_ATTRIBUTE_STATIC))
        {
            std::stringstream message;
            message << "Field '" << fieldNameToFind << "' is not a marshaled member of the type '" << type->name << "'";
            vm::Exception::Raise(vm::Exception::GetArgumentException("fieldName", message.str().c_str()));
        }

        // Order the base classes so the most base class is first.
        std::deque<Il2CppClass*> inheritanceHierarchy;
        while (type)
        {
            inheritanceHierarchy.push_front(type);
            type = type->parent;
        }

        bool fieldFound = false;

        size_t offset = 0;
        FieldInfo* previousField = NULL;
        for (std::deque<Il2CppClass*>::iterator it = inheritanceHierarchy.begin(); it < inheritanceHierarchy.end(); ++it)
        {
            type = *it;
            void* iter = NULL;
            while ((field = vm::Class::GetFields(type, &iter)))
            {
                if (vm::Field::GetFlags(field) & FIELD_ATTRIBUTE_STATIC)
                    continue;

                // Determine how much the previous field added to the offset.
                if (previousField != NULL)
                {
                    if (!Type::IsStruct(previousField->type))
                    {
                        size_t managedOffset = field->offset - previousField->offset;
                        if (type->packingSize == 0)
                            offset += managedOffset;
                        else if (managedOffset != 0) // overlapping fields have a zero offset
                        {
                            offset += vm::Class::GetFieldMarshaledSize(previousField);
                            offset = RoundUpToMultiple(offset, std::min((int)type->packingSize, vm::Class::GetFieldMarshaledSize(field)));
                        }
                    }
                    else
                    {
                        offset += Class::FromIl2CppType(previousField->type)->native_size;
                    }
                }
                previousField = field;

                if (fieldNameToFind == vm::Field::GetName(field))
                {
                    fieldFound = true;
                    break;
                }
            }

            if (fieldFound)
                break;
        }

        Il2CppIntPtr offsetPtr;
        offsetPtr.m_value = (void*)(offset);
        return offsetPtr;
    }

    void Marshal::Prelink(Il2CppReflectionMethod* m)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::Prelink, "This icall is not supported by il2cpp.");
    }

    void Marshal::PrelinkAll(Il2CppReflectionType* c)
    {
        NOT_SUPPORTED_IL2CPP(Marshal::PrelinkAll, "This icall is not supported by il2cpp.");
    }

    Il2CppIntPtr Marshal::ReAllocCoTaskMem(Il2CppIntPtr ptr, int32_t size)
    {
        Il2CppIntPtr result;
        result.m_value = MarshalAlloc::ReAlloc(ptr.m_value, size);
        return result;
    }

    Il2CppIntPtr Marshal::ReAllocHGlobal(Il2CppIntPtr ptr, Il2CppIntPtr size)
    {
        if (ptr.m_value == NULL)
            Exception::RaiseOutOfMemoryException();

        Il2CppIntPtr result;
        result.m_value = MarshalAlloc::ReAllocHGlobal(ptr.m_value, (size_t)size.m_value);
        return result;
    }

    Il2CppIntPtr Marshal::UnsafeAddrOfPinnedArrayElement(Il2CppArray* arr, int32_t index)
    {
        Il2CppIntPtr address;
        address.m_value = il2cpp_array_addr_with_size(arr, il2cpp_array_element_size(arr->klass), index);
        return address;
    }

#if NET_4_0

    Il2CppIntPtr Marshal::BufferToBSTR(Il2CppArray* ptr, int32_t slen)
    {
        NOT_IMPLEMENTED_ICALL(Marshal::BufferToBSTR);
        IL2CPP_UNREACHABLE;
    }

    int32_t Marshal::GetHRForException_WinRT(Il2CppException* e)
    {
        NOT_IMPLEMENTED_ICALL(Marshal::GetHRForException_WinRT);
        IL2CPP_UNREACHABLE;
    }

    Il2CppIntPtr Marshal::GetRawIUnknownForComObjectNoAddRef(Il2CppObject* o)
    {
        IL2CPP_ASSERT(o->klass->is_import_or_windows_runtime);

        Il2CppIntPtr result = { static_cast<Il2CppComObject*>(o)->identity };
        return result;
    }

    Il2CppObject* Marshal::GetNativeActivationFactory(Il2CppObject* type)
    {
        NOT_IMPLEMENTED_ICALL(Marshal::GetNativeActivationFactory);
        IL2CPP_UNREACHABLE;
    }

#endif
} /* namespace InteropServices */
} /* namespace Runtime */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
