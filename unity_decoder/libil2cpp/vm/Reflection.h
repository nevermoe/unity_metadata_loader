#pragma once

#include <stdint.h>
#include "il2cpp-config.h"

struct Il2CppString;
struct Il2CppArray;
struct Il2CppReflectionAssembly;
struct Il2CppReflectionAssemblyName;
struct Il2CppReflectionField;
struct Il2CppReflectionMethod;
struct Il2CppReflectionModule;
struct Il2CppReflectionProperty;
struct Il2CppReflectionEvent;
struct Il2CppReflectionType;
struct Il2CppReflectionParameter;
struct Il2CppClass;
struct FieldInfo;
struct MethodInfo;
struct PropertyInfo;
struct EventInfo;
struct Il2CppClass;
struct CustomAttributesCache;
struct CustomAttributeTypeCache;
struct Il2CppAssembly;
struct Il2CppAssemblyName;
struct Il2CppImage;
struct Il2CppType;
struct Il2CppObject;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API Reflection
{
// exported
public:
	static Il2CppReflectionAssembly* GetAssemblyObject (const Il2CppAssembly *assembly);
	static Il2CppReflectionAssemblyName* GetAssemblyNameObject (const Il2CppAssemblyName *assemblyName);
	static Il2CppReflectionField* GetFieldObject (Il2CppClass *klass, FieldInfo *field);
	static Il2CppReflectionProperty* GetPropertyObject (Il2CppClass *klass, const PropertyInfo *property);
	static Il2CppReflectionEvent* GetEventObject(Il2CppClass *klass, const EventInfo *event);
	static Il2CppReflectionMethod* GetMethodObject (const MethodInfo *method, Il2CppClass *refclass);
	static Il2CppReflectionModule* GetModuleObject (const Il2CppImage *image);
	static Il2CppReflectionType* GetTypeObject (const Il2CppType *type);
	static Il2CppArray* GetParamObjects (const MethodInfo *method, Il2CppClass *refclass);
	static CustomAttributesCache* GetCustomAttrsInfo (Il2CppObject *obj);

	static bool HasAttribute (Il2CppObject *obj, Il2CppClass *attribute);
	static bool HasAttribute (FieldInfo *field, Il2CppClass *attribute);
	static bool HasAttribute (const MethodInfo *method, Il2CppClass *attribute);
	static bool HasAttribute (Il2CppClass *klass, Il2CppClass *attribute);

	static bool IsType (Il2CppObject *obj);
	static bool IsField (Il2CppObject *obj);
	static bool IsAnyMethod (Il2CppObject *obj);
	static bool IsProperty(Il2CppObject *obj);
	static bool IsEvent (Il2CppObject *obj);

// internal
public:
	static void Initialize ();
	static bool CustomAttrsHasAttr (CustomAttributeTypeCache *ainfo, Il2CppClass *attr_klass);
	static Il2CppClass* TypeGetHandle (Il2CppReflectionType* ref);
	static Il2CppObject* GetDBNullObject ();

private:
	static CustomAttributeTypeCache* GetCustomAttrsTypeInfo (Il2CppObject *obj);
	static CustomAttributeTypeCache* GetCustomAttributeTypeCacheFor (Il2CppReflectionParameter *parameter);
	static CustomAttributesCache* GetCustomAttributesCacheFor (Il2CppClass *klass);
	static CustomAttributesCache* GetCustomAttributesCacheFor (const MethodInfo *method);
	static CustomAttributesCache* GetCustomAttributesCacheFor (const PropertyInfo *property);
	static CustomAttributesCache* GetCustomAttributesCacheFor (FieldInfo *field);
	static CustomAttributesCache* GetCustomAttributesCacheFor (const EventInfo *event);
	static CustomAttributesCache* GetCustomAttributesCacheFor (Il2CppReflectionParameter *param);
	static CustomAttributesCache* GetCustomAttributesCacheFor (const Il2CppAssembly *assembly);
};

} /* namespace vm */
} /* namespace il2cpp */
