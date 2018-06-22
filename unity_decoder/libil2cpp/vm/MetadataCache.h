#pragma once

#include <stdint.h>
#include <vector>
#include "il2cpp-config.h"
#include "Assembly.h"
#include "metadata/Il2CppTypeVector.h"
#include "class-internals.h"

struct MethodInfo;
struct Il2CppClass;
struct Il2CppGenericContainer;
struct Il2CppGenericContext;
struct Il2CppGenericInst;
struct Il2CppGenericMethod;
struct Il2CppType;
struct Il2CppString;

namespace il2cpp
{
namespace vm
{
    class LIBIL2CPP_CODEGEN_API MetadataCache
    {
    public:

        static void Register(const Il2CppCodeRegistration * const codeRegistration, const Il2CppMetadataRegistration * const metadataRegistration, const Il2CppCodeGenOptions* const codeGenOptions);

        static void Initialize();
        static void InitializeGCSafe();

        static Il2CppClass* GetGenericInstanceType(Il2CppClass* genericTypeDefinition, const il2cpp::metadata::Il2CppTypeVector& genericArgumentTypes);
        static const MethodInfo* GetGenericInstanceMethod(const MethodInfo* genericMethodDefinition, const Il2CppGenericContext* context);
        static const MethodInfo* GetGenericInstanceMethod(const MethodInfo* genericMethodDefinition, const il2cpp::metadata::Il2CppTypeVector& genericArgumentTypes);
        static const Il2CppGenericContext* GetMethodGenericContext(const MethodInfo* method);
        static const Il2CppGenericContainer* GetMethodGenericContainer(const MethodInfo* method);
        static const MethodInfo* GetGenericMethodDefinition(const MethodInfo* method);

        static Il2CppClass* GetPointerType(Il2CppClass* type);
        static Il2CppClass* GetWindowsRuntimeClass(const std::string& fullName);
        static const char* GetWindowsRuntimeClassName(const Il2CppClass* klass);
        static void AddPointerType(Il2CppClass* type, Il2CppClass* pointerType);

        static const Il2CppGenericInst* GetGenericInst(const Il2CppType* const* types, uint32_t typeCount);
        static const Il2CppGenericInst* GetGenericInst(const il2cpp::metadata::Il2CppTypeVector& types);
        static const Il2CppGenericMethod* GetGenericMethod(const MethodInfo* methodDefinition, const Il2CppGenericInst* classInst, const Il2CppGenericInst* methodInst);

        static InvokerMethod GetInvokerMethodPointer(const MethodInfo* methodDefinition, const Il2CppGenericContext* context);
        static Il2CppMethodPointer GetMethodPointer(const MethodInfo* methodDefinition, const Il2CppGenericContext* context);

        static Il2CppClass* GetTypeInfoFromTypeIndex(TypeIndex index);
        static const Il2CppType* GetIl2CppTypeFromIndex(TypeIndex index);
        static const MethodInfo* GetMethodInfoFromIndex(EncodedMethodIndex index);
        static const Il2CppGenericMethod* GetGenericMethodFromIndex(GenericMethodIndex index);
        static Il2CppString* GetStringLiteralFromIndex(StringLiteralIndex index);
        static const char* GetStringFromIndex(StringIndex index);

        static FieldInfo* GetFieldInfoFromIndex(EncodedMethodIndex index);
        static void InitializeMethodMetadata(uint32_t index);

        static Il2CppMethodPointer GetMethodPointerFromIndex(MethodIndex index);
        static InvokerMethod GetMethodInvokerFromIndex(MethodIndex index);
        static const Il2CppInteropData* GetInteropDataForType(const Il2CppType* type);
        static Il2CppMethodPointer GetReversePInvokeWrapperFromIndex(MethodIndex index);

        static Il2CppMethodPointer GetUnresolvedVirtualCallStub(const MethodInfo* method);

        static const Il2CppAssembly* GetAssemblyFromIndex(AssemblyIndex index);
        static const Il2CppAssembly* GetAssemblyByName(const std::string& name);
        static Il2CppImage* GetImageFromIndex(ImageIndex index);
        static Il2CppClass* GetTypeInfoFromTypeDefinitionIndex(TypeDefinitionIndex index);
        static const Il2CppTypeDefinition* GetTypeDefinitionFromIndex(TypeDefinitionIndex index);
        static TypeDefinitionIndex GetExportedTypeFromIndex(TypeDefinitionIndex index);
        static const Il2CppGenericContainer* GetGenericContainerFromIndex(GenericContainerIndex index);
        static const Il2CppGenericParameter* GetGenericParameterFromIndex(GenericParameterIndex index);
        static const Il2CppType* GetGenericParameterConstraintFromIndex(GenericParameterConstraintIndex index);
        static Il2CppClass* GetNestedTypeFromIndex(NestedTypeIndex index);
        static const Il2CppType* GetInterfaceFromIndex(InterfacesIndex index);
        static EncodedMethodIndex GetVTableMethodFromIndex(VTableIndex index);
        static Il2CppInterfaceOffsetPair GetInterfaceOffsetIndex(InterfaceOffsetIndex index);
        static const Il2CppRGCTXDefinition* GetRGCTXDefinitionFromIndex(RGCTXIndex index);
        static const Il2CppEventDefinition* GetEventDefinitionFromIndex(EventIndex index);
        static const Il2CppFieldDefinition* GetFieldDefinitionFromIndex(FieldIndex index);
        static const Il2CppFieldDefaultValue* GetFieldDefaultValueFromIndex(FieldIndex index);
        static const uint8_t* GetFieldDefaultValueDataFromIndex(FieldIndex index);
        static const Il2CppFieldDefaultValue* GetFieldDefaultValueForField(const FieldInfo* field);
        static const uint8_t* GetParameterDefaultValueDataFromIndex(ParameterIndex index);
        static const Il2CppParameterDefaultValue* GetParameterDefaultValueForParameter(const MethodInfo* method, const ParameterInfo* parameter);
        static int GetFieldMarshaledSizeForField(const FieldInfo* field);
        static const Il2CppMethodDefinition* GetMethodDefinitionFromIndex(MethodIndex index);
        static const MethodInfo* GetMethodInfoFromMethodDefinitionIndex(MethodIndex index);
        static const Il2CppPropertyDefinition* GetPropertyDefinitionFromIndex(PropertyIndex index);
        static const Il2CppParameterDefinition* GetParameterDefinitionFromIndex(ParameterIndex index);

        // returns the compiler computer field offset for type definition fields
        static int32_t GetFieldOffsetFromIndex(TypeIndex typeIndex, int32_t fieldIndexInType);

        static int32_t GetReferenceAssemblyIndexIntoAssemblyTable(int32_t referencedAssemblyTableIndex);

        static const TypeDefinitionIndex GetIndexForTypeDefinition(const Il2CppClass* typeDefinition);
        static const GenericParameterIndex GetIndexForGenericParameter(const Il2CppGenericParameter* genericParameter);

        static CustomAttributesCache* GenerateCustomAttributesCache(CustomAttributeIndex index);
        static CustomAttributeTypeCache* GenerateCustomAttributeTypeCache(CustomAttributeIndex index);

        typedef void(*WalkTypesCallback)(Il2CppClass* type, void* context);
        static void WalkPointerTypes(WalkTypesCallback callback, void* context);

    private:
        static void InitializeUnresolvedSignatureTable();
        static void InitializeStringLiteralTable();
        static void InitializeGenericMethodTable();
        static void InitializeWindowsRuntimeTypeNamesTables();
    };
} // namespace vm
} // namespace il2cpp
