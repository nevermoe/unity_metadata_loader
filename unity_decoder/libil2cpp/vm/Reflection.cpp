#include "il2cpp-config.h"
#include "class-internals.h"
#include "object-internals.h"
#include "tabledefs.h"
#include <cassert>
#include "gc/GCHandle.h"
#include "metadata/Il2CppTypeCompare.h"
#include "metadata/Il2CppTypeHash.h"
#include "metadata/Il2CppTypeLess.h"
#include "os/Mutex.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Field.h"
#include "vm/Image.h"
#include "vm/MetadataCache.h"
#include "vm/Object.h"
#include "vm/Parameter.h"
#include "vm/Reflection.h"
#include "vm/String.h"
#include "vm/AssemblyName.h"
#include "utils/StdUnorderedMap.h"
#include "utils/StringUtils.h"


#include "gc/Allocator.h"

using il2cpp::metadata::Il2CppTypeCompare;
using il2cpp::metadata::Il2CppTypeHash;
using il2cpp::metadata::Il2CppTypeLess;

template<typename T>
struct ReflectionMapHash
{
	size_t operator( )(const T& ea) const
	{
		return ((size_t)(intptr_t)(ea.first)) >> 3;
	}
};

template<typename T>
struct ReflectionMapEquals
{
	bool operator()(const T& ea, const T& eb) const
	{
		return (ea.first == eb.first) && (ea.second == eb.second);
	}
};

template<typename T>
struct ReflectionMapLess
{
	bool operator()(const T& ea, const T& eb) const
	{
		if (ea.first < eb.first)
			return true;
		if (ea.second < eb.second)
			return true;
		return false;
	}
};

template <typename Key, typename Value>
struct ReflectionMap : unordered_map<
	Key,
	Value,
#if IL2CPP_HAS_UNORDERED_CONTAINER
	ReflectionMapHash<Key>,
	ReflectionMapEquals<Key>,
#else
	ReflectionMapLess<Key>,
#endif
	il2cpp::gc::Allocator<std::pair<const Key, Value> > >
{
};

typedef ReflectionMap<std::pair<const Il2CppAssembly*, Il2CppClass*>, Il2CppReflectionAssembly*> AssemblyMap;
typedef ReflectionMap<std::pair<const FieldInfo*, Il2CppClass*>, Il2CppReflectionField*> FieldMap;
typedef ReflectionMap<std::pair<const PropertyInfo*, Il2CppClass*>, Il2CppReflectionProperty*> PropertyMap;
typedef ReflectionMap<std::pair<const EventInfo*, Il2CppClass*>, Il2CppReflectionEvent*> EventMap;
typedef ReflectionMap<std::pair<const MethodInfo*, Il2CppClass*>, Il2CppReflectionMethod*> MethodMap;
typedef ReflectionMap<std::pair<const Il2CppImage*, Il2CppClass*>, Il2CppReflectionModule*> ModuleMap;
typedef ReflectionMap<std::pair<const MethodInfo*, Il2CppClass*>, Il2CppArray*> ParametersMap;

typedef const Il2CppType* TypeMapKey;
typedef Il2CppReflectionType* TypeMapValue;
typedef unordered_map<TypeMapKey,TypeMapValue,
#if IL2CPP_HAS_UNORDERED_CONTAINER
	Il2CppTypeHash,
	Il2CppTypeCompare,
#else
	Il2CppTypeLess,
#endif
	il2cpp::gc::Allocator<std::pair<const TypeMapKey, TypeMapValue> > > TypeMap;

// these needs to be pointers and allocated after GC is initialized since it uses GC Allocator
static AssemblyMap* s_AssemblyMap;
static FieldMap* s_FieldMap;
static PropertyMap* s_PropertyMap;
static EventMap* s_EventMap;
static MethodMap* s_MethodMap;
static ModuleMap* s_ModuleMap;
static ParametersMap* s_ParametersMap;
static TypeMap* s_TypeMap;

namespace il2cpp
{
namespace vm
{

static il2cpp::os::FastMutex s_ReflectionICallsMutex;

Il2CppReflectionAssembly* Reflection::GetAssemblyObject (const Il2CppAssembly *assembly)
{
	static Il2CppClass *System_Reflection_Assembly;
	Il2CppReflectionAssembly *res;
	
	il2cpp::os::FastAutoLock lock(&s_ReflectionICallsMutex);

	AssemblyMap::key_type key (assembly, (Il2CppClass*)NULL);
	AssemblyMap::iterator iter = s_AssemblyMap->find (key);
	if (iter != s_AssemblyMap->end ())
		return iter->second;

	if (!System_Reflection_Assembly)
		System_Reflection_Assembly = Class::FromName (
			il2cpp_defaults.corlib, "System.Reflection", "Assembly");
	res = (Il2CppReflectionAssembly *)Object::New (System_Reflection_Assembly);
	res->assembly = assembly;

	s_AssemblyMap->insert (AssemblyMap::value_type (key, res));

	return res;
}

Il2CppReflectionAssemblyName* Reflection::GetAssemblyNameObject (const Il2CppAssemblyName *assemblyName)
{
	std::string fullAssemblyName = vm::AssemblyName::AssemblyNameToString (*assemblyName);
	Il2CppReflectionAssemblyName* reflectionAssemblyName = (Il2CppReflectionAssemblyName*)Object::New (il2cpp_defaults.assembly_name_class);
	vm::AssemblyName::ParseName (reflectionAssemblyName, fullAssemblyName);
	return reflectionAssemblyName;
}

Il2CppReflectionField* Reflection::GetFieldObject (Il2CppClass *klass, FieldInfo *field)
{
	Il2CppReflectionField *res;
	static Il2CppClass *monofield_klass;

	il2cpp::os::FastAutoLock lock(&s_ReflectionICallsMutex);

	FieldMap::key_type key (field, klass);
	FieldMap::iterator iter = s_FieldMap->find (key);
	if (iter != s_FieldMap->end ())
		return iter->second;

	if (!monofield_klass)
		monofield_klass = Class::FromName (il2cpp_defaults.corlib, "System.Reflection", "MonoField");
	res = (Il2CppReflectionField *)Object::New (monofield_klass);
	res->klass = klass;
	res->field = field;
	IL2CPP_OBJECT_SETREF (res, name, String::New (Field::GetName (field)));
	res->attrs = field->type->attrs;
	IL2CPP_OBJECT_SETREF (res, type, GetTypeObject (field->type));

	s_FieldMap->insert (FieldMap::value_type (key, res));

	return res;
}

/*
 * We use the same C representation for methods and constructors, but the type
 * name in C# is different.
 */
static Il2CppClass *System_Reflection_MonoMethod = NULL;
static Il2CppClass *System_Reflection_MonoCMethod = NULL;

static Il2CppClass *System_Reflection_MonoGenericCMethod = NULL;
static Il2CppClass *System_Reflection_MonoGenericMethod = NULL;

Il2CppReflectionMethod* Reflection::GetMethodObject (const MethodInfo *method, Il2CppClass *refclass)
{
	Il2CppClass *klass;
	Il2CppReflectionMethod *ret;

	il2cpp::os::FastAutoLock lock(&s_ReflectionICallsMutex);

	if (method->is_inflated)
	{
		refclass = method->declaring_type;

		MethodMap::key_type key (method, refclass);
		MethodMap::iterator iter = s_MethodMap->find (key);
		if (iter != s_MethodMap->end ())
			return iter->second;

		if ((*method->name == '.') && (!strcmp (method->name, ".ctor") || !strcmp (method->name, ".cctor")))
		{
			if (!System_Reflection_MonoGenericCMethod)
				System_Reflection_MonoGenericCMethod = Class::FromName (il2cpp_defaults.corlib, "System.Reflection", "MonoGenericCMethod");
			klass = System_Reflection_MonoGenericCMethod;
		} else {
			if (!System_Reflection_MonoGenericMethod)
				System_Reflection_MonoGenericMethod = Class::FromName (il2cpp_defaults.corlib, "System.Reflection", "MonoGenericMethod");
			klass = System_Reflection_MonoGenericMethod;
		}

		Il2CppReflectionGenericMethod *gret = (Il2CppReflectionGenericMethod*)Object::New (klass);
		gret->base.method = method;

		IL2CPP_OBJECT_SETREF (gret, base.name, String::New (method->name));
		IL2CPP_OBJECT_SETREF (gret, base.reftype, GetTypeObject (refclass->byval_arg));

		ret = &gret->base;

		s_MethodMap->insert (MethodMap::value_type (key, ret));

		return ret;
	}

	if (!refclass)
		refclass = method->declaring_type;

	MethodMap::key_type key (method, refclass);
	MethodMap::iterator iter = s_MethodMap->find (key);
	if (iter != s_MethodMap->end ())
		return iter->second;

	if (*method->name == '.' && (strcmp (method->name, ".ctor") == 0 || strcmp (method->name, ".cctor") == 0)) {
		if (!System_Reflection_MonoCMethod)
			System_Reflection_MonoCMethod = Class::FromName (il2cpp_defaults.corlib, "System.Reflection", "MonoCMethod");
		klass = System_Reflection_MonoCMethod;
	}
	else {
		if (!System_Reflection_MonoMethod)
			System_Reflection_MonoMethod = Class::FromName (il2cpp_defaults.corlib, "System.Reflection", "MonoMethod");
		klass = System_Reflection_MonoMethod;
	}
	ret = (Il2CppReflectionMethod*)Object::New (klass);
	ret->method = method;
	IL2CPP_OBJECT_SETREF (ret, reftype, GetTypeObject (refclass->byval_arg));

	s_MethodMap->insert (MethodMap::value_type (key, ret));

	return ret;
}

Il2CppReflectionModule* Reflection::GetModuleObject (const Il2CppImage *image)
{
	static Il2CppClass *System_Reflection_Module;
	Il2CppReflectionModule *res;
	//char* basename;

	il2cpp::os::FastAutoLock lock(&s_ReflectionICallsMutex);

	ModuleMap::key_type key (image, (Il2CppClass*)NULL);
	ModuleMap::iterator iter = s_ModuleMap->find (key);
	if (iter != s_ModuleMap->end ())
		return iter->second;

	if (!System_Reflection_Module)
		System_Reflection_Module = Class::FromName (il2cpp_defaults.corlib, "System.Reflection", "Module");
	res = (Il2CppReflectionModule *)Object::New (System_Reflection_Module);

	res->image = image;
	IL2CPP_OBJECT_SETREF (res, assembly, (Il2CppReflectionAssembly *) Reflection::GetAssemblyObject (MetadataCache::GetAssemblyFromIndex (image->assemblyIndex)));

	IL2CPP_OBJECT_SETREF (res, fqname, String::New (image->name));
	NOT_IMPLEMENTED_ICALL_NO_ASSERT (Reflection::GetModuleObject, "Missing Module fields need set");
	//basename = g_path_get_basename (image->name);
	//IL2CPP_OBJECT_SETREF (res, name, String::New (basename));
	IL2CPP_OBJECT_SETREF (res, name, String::New (image->name));
	//IL2CPP_OBJECT_SETREF (res, scopename, String::New (image->module_name));
	
	//g_free (basename);

	/*if (image->assembly->image == image) {
		res->token = mono_metadata_make_token (MONO_TABLE_MODULE, 1);
	} else {
		int i;
		res->token = 0;
		if (image->assembly->image->modules) {
			for (i = 0; i < image->assembly->image->module_count; i++) {
				if (image->assembly->image->modules [i] == image)
					res->token = mono_metadata_make_token (MONO_TABLE_MODULEREF, i + 1);
			}
			g_assert (res->token);
		}
	}*/

	s_ModuleMap->insert (ModuleMap::value_type (key, res));
	return res;
}

Il2CppReflectionProperty* Reflection::GetPropertyObject (Il2CppClass *klass, const PropertyInfo *property)
{
	Il2CppReflectionProperty *res;
	static Il2CppClass *monoproperty_klass;

	il2cpp::os::FastAutoLock lock(&s_ReflectionICallsMutex);

	PropertyMap::key_type key (property, klass);
	PropertyMap::iterator iter = s_PropertyMap->find (key);
	if (iter != s_PropertyMap->end ())
		return iter->second;

	if (!monoproperty_klass)
		monoproperty_klass = Class::FromName (il2cpp_defaults.corlib, "System.Reflection", "MonoProperty");
	res = (Il2CppReflectionProperty *)Object::New (monoproperty_klass);
	res->klass = klass;
	res->property = property;

	s_PropertyMap->insert (PropertyMap::value_type (key, res));

	return res;
}

Il2CppReflectionEvent* Reflection::GetEventObject(Il2CppClass* klass, const EventInfo* event)
{
	Il2CppReflectionEvent* result;
	static Il2CppClass* monoproperty_klass = Class::FromName(il2cpp_defaults.corlib, "System.Reflection", "MonoEvent");

	il2cpp::os::FastAutoLock lock(&s_ReflectionICallsMutex);

	EventMap::key_type key (event, klass);
	EventMap::iterator iter = s_EventMap->find (key);
	if (iter != s_EventMap->end ())
		return iter->second;

	Il2CppReflectionMonoEvent* monoEvent = reinterpret_cast<Il2CppReflectionMonoEvent*>(Object::New(monoproperty_klass));
	monoEvent->eventInfo = event;
	monoEvent->reflectedType = Reflection::GetTypeObject(klass->byval_arg);
	result = reinterpret_cast<Il2CppReflectionEvent*>(monoEvent);

	s_EventMap->insert (EventMap::value_type (key, result));

	return result;
}

Il2CppReflectionType* Reflection::GetTypeObject (const Il2CppType *type)
{
	il2cpp::os::FastAutoLock lock(&s_ReflectionICallsMutex);

	TypeMap::iterator iter = s_TypeMap->find (type);
	if (iter != s_TypeMap->end ())
		return iter->second;

	Il2CppReflectionType* typeObject = (Il2CppReflectionType*)Object::New (il2cpp_defaults.monotype_class);
	typeObject->type = type;

	s_TypeMap->insert (TypeMap::value_type (type, typeObject));

	return typeObject;
}

Il2CppObject* Reflection::GetDBNullObject ()
{
	Il2CppObject* valueFieldValue;
	static FieldInfo *dbNullValueField = NULL;

	if (!dbNullValueField)
	{
		dbNullValueField = Class::GetFieldFromName(il2cpp_defaults.dbnull_class, "Value");
		assert(dbNullValueField);
	}

	valueFieldValue = Field::GetValueObject(dbNullValueField, NULL);
	assert(valueFieldValue);
	return valueFieldValue;
}

static Il2CppObject* GetReflectionMissingObject()
{
	Il2CppObject* valueFieldValue;
	static FieldInfo *reflectionMissingField = NULL;

	if (!reflectionMissingField)
	{
		Il2CppClass* klass = Image::ClassFromName(il2cpp_defaults.corlib, "System.Reflection", "Missing");
		Class::Init(klass);
		reflectionMissingField = Class::GetFieldFromName(klass, "Value");
		assert(reflectionMissingField);
	}

	valueFieldValue = Field::GetValueObject(reflectionMissingField, NULL);
	assert(valueFieldValue);
	return valueFieldValue;
}

static Il2CppObject* GetObjectForMissingDefaultValue(uint32_t parameterAttributes)
{
	if (parameterAttributes & PARAM_ATTRIBUTE_OPTIONAL)
		return GetReflectionMissingObject();
	else
		return Reflection::GetDBNullObject();
}

Il2CppArray* Reflection::GetParamObjects (const MethodInfo *method, Il2CppClass *refclass)
{
	static Il2CppClass *System_Reflection_ParameterInfo;
	static Il2CppClass *System_Reflection_ParameterInfo_array;
	Il2CppArray *res = NULL;
	Il2CppReflectionMethod *member = NULL;

	il2cpp::os::FastAutoLock lock(&s_ReflectionICallsMutex);

	NOT_IMPLEMENTED_NO_ASSERT (Reflection::GetParamObjects, "Work in progress!");

	if (!System_Reflection_ParameterInfo_array) {
		Il2CppClass *klass;

		klass = Class::FromName (il2cpp_defaults.corlib, "System.Reflection", "ParameterInfo");
		//mono_memory_barrier ();
		System_Reflection_ParameterInfo = klass;
	
		klass = Class::GetArrayClass (klass, 1);
		//mono_memory_barrier ();
		System_Reflection_ParameterInfo_array = klass;
	}

	if (!method->parameters_count)
		return Array::NewSpecific (System_Reflection_ParameterInfo_array, 0);
	
	// Mono caches based on the address of the method pointer in the MethodInfo
	// since they put everything in one cache and the MethodInfo is already used as key for GetMethodObject caching
	// However, since we have distinct maps for the different types we can use MethodInfo as the key again

	ParametersMap::key_type key (method, refclass);
	ParametersMap::iterator iter = s_ParametersMap->find (key);
	if (iter != s_ParametersMap->end ())
		return iter->second;
	
	member = GetMethodObject (method, refclass);
	res = Array::NewSpecific (System_Reflection_ParameterInfo_array, method->parameters_count);
	for (int i = 0; i < method->parameters_count; ++i) {
		Il2CppReflectionParameter* param = (Il2CppReflectionParameter *)Object::New (System_Reflection_ParameterInfo);
		IL2CPP_OBJECT_SETREF (param, ClassImpl, GetTypeObject (method->parameters [i].parameter_type));
		IL2CPP_OBJECT_SETREF (param, MemberImpl, (Il2CppObject*)member);
		IL2CPP_OBJECT_SETREF (param, NameImpl, method->parameters[i].name ? String::New (method->parameters[i].name) : NULL);
		param->PositionImpl = i;
		param->AttrsImpl = method->parameters [i].parameter_type->attrs;

		Il2CppObject* defaultValue = NULL;
		if (param->AttrsImpl & PARAM_ATTRIBUTE_HAS_DEFAULT)
		{
			bool isExplicitySetNullDefaultValue = false;
			defaultValue = Parameter::GetDefaultParameterValueObject(method, &method->parameters[i], &isExplicitySetNullDefaultValue);
			if (defaultValue == NULL && !isExplicitySetNullDefaultValue)
				defaultValue = GetObjectForMissingDefaultValue(param->AttrsImpl);
		}
		else
		{
			defaultValue = GetObjectForMissingDefaultValue(param->AttrsImpl);
		}

		IL2CPP_OBJECT_SETREF(param, DefaultValueImpl, defaultValue);

		il2cpp_array_setref(res, i, param);
	}

	s_ParametersMap->insert (ParametersMap::value_type (key, res));

	return res;
}

// TODO: move this somewhere else
bool Reflection::IsType(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "MonoType") == 0 && strcmp(obj->klass->namespaze, "System") == 0;
	return false;
}

static bool IsMethod(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "MonoMethod") == 0 && strcmp(obj->klass->namespaze, "System.Reflection") == 0;
	return false;
}

static bool IsCMethod(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "MonoCMethod") == 0 && strcmp(obj->klass->namespaze, "System.Reflection") == 0;
	return false;
}

static bool IsGenericMethod(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "MonoGenericMethod") == 0 && strcmp(obj->klass->namespaze, "System.Reflection") == 0;
	return false;
}

static bool IsGenericCMethod(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "MonoGenericCMethod") == 0 && strcmp(obj->klass->namespaze, "System.Reflection") == 0;
	return false;
}

bool Reflection::IsAnyMethod (Il2CppObject *obj)
{
	return IsMethod (obj) || IsCMethod (obj) || IsGenericMethod (obj) || IsGenericCMethod (obj);
}

bool Reflection::IsField(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "MonoField") == 0 && strcmp(obj->klass->namespaze, "System.Reflection") == 0;
	return false;
}

bool Reflection::IsProperty(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "MonoProperty") == 0 && strcmp(obj->klass->namespaze, "System.Reflection") == 0;
	return false;
}

bool Reflection::IsEvent(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "MonoEvent") == 0 && strcmp(obj->klass->namespaze, "System.Reflection") == 0;
	return false;
}

static bool IsParameter(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "ParameterInfo") == 0 && strcmp(obj->klass->namespaze, "System.Reflection") == 0;
	return false;
}

static bool IsAssembly(Il2CppObject *obj)
{
	if (obj->klass->image == il2cpp_defaults.corlib)
		return strcmp(obj->klass->name, "Assembly") == 0 && strcmp(obj->klass->namespaze, "System.Reflection") == 0;
	return false;
}

CustomAttributesCache* Reflection::GetCustomAttributesCacheFor (Il2CppClass *klass)
{
	return MetadataCache::GenerateCustomAttributesCache (klass->customAttributeIndex);
}

CustomAttributesCache* Reflection::GetCustomAttributesCacheFor (const MethodInfo *method)
{
	return MetadataCache::GenerateCustomAttributesCache (method->customAttributeIndex);
}

CustomAttributesCache* Reflection::GetCustomAttributesCacheFor (const PropertyInfo *property)
{
	return MetadataCache::GenerateCustomAttributesCache (property->customAttributeIndex);
}

CustomAttributesCache* Reflection::GetCustomAttributesCacheFor (FieldInfo *field)
{
	return MetadataCache::GenerateCustomAttributesCache (field->customAttributeIndex);
}

CustomAttributesCache* Reflection::GetCustomAttributesCacheFor(const EventInfo *event)
{
	return MetadataCache::GenerateCustomAttributesCache (event->customAttributeIndex);
}

CustomAttributesCache* Reflection::GetCustomAttributesCacheFor(Il2CppReflectionParameter *parameter)
{
	Il2CppReflectionMethod* method = (Il2CppReflectionMethod*)parameter->MemberImpl;

	if(method->method->parameters == NULL)
		return NULL;

	NOT_IMPLEMENTED_NO_ASSERT (Reflection::GetCustomAttributesCacheFor, "-1 represents the return value. Need to emit custom attribute information for that.")
	if (parameter->PositionImpl == -1)
		return NULL;

	const ::ParameterInfo* info = &method->method->parameters[parameter->PositionImpl];
	return MetadataCache::GenerateCustomAttributesCache (info->customAttributeIndex);
}

CustomAttributeTypeCache* Reflection::GetCustomAttributeTypeCacheFor(Il2CppReflectionParameter *parameter)
{
	Il2CppReflectionMethod* method = (Il2CppReflectionMethod*)parameter->MemberImpl;

	if (method->method->parameters == NULL)
		return NULL;

	NOT_IMPLEMENTED_NO_ASSERT(Reflection::GetCustomAttributeTypeCacheFor, "-1 represents the return value. Need to emit custom attribute information for that.")
		if (parameter->PositionImpl == -1)
			return NULL;

	const ::ParameterInfo* info = &method->method->parameters[parameter->PositionImpl];
	return MetadataCache::GenerateCustomAttributeTypeCache(info->customAttributeIndex);
}

CustomAttributesCache* Reflection::GetCustomAttributesCacheFor(const Il2CppAssembly *assembly)
{
	return MetadataCache::GenerateCustomAttributesCache (assembly->customAttributeIndex);
}

CustomAttributesCache* Reflection::GetCustomAttrsInfo (Il2CppObject *obj)
{
	if (IsMethod(obj) || IsCMethod(obj) || IsGenericMethod(obj) || IsGenericCMethod(obj))
		return GetCustomAttributesCacheFor (((Il2CppReflectionMethod*)obj)->method);

	if(IsProperty (obj))
		return GetCustomAttributesCacheFor (((Il2CppReflectionProperty*)obj)->property);

	if(IsField (obj))
		return GetCustomAttributesCacheFor (((Il2CppReflectionField*)obj)->field);

	if (IsEvent(obj))
		return GetCustomAttributesCacheFor(((Il2CppReflectionMonoEvent*)obj)->eventInfo);

	if (IsParameter(obj))
		return GetCustomAttributesCacheFor(((Il2CppReflectionParameter*)obj));

	if (IsAssembly(obj))
		return GetCustomAttributesCacheFor(((Il2CppReflectionAssembly*)obj)->assembly);

	Il2CppClass *klass = IsType (obj)
		? Class::FromSystemType ((Il2CppReflectionType*)obj)
		: obj->klass;

	return GetCustomAttributesCacheFor (klass);

}

CustomAttributeTypeCache* Reflection::GetCustomAttrsTypeInfo (Il2CppObject *obj)
{
	if (IsMethod (obj) || IsCMethod (obj) || IsGenericMethod (obj) || IsGenericCMethod (obj))
		return MetadataCache::GenerateCustomAttributeTypeCache ((((Il2CppReflectionMethod*)obj)->method)->customAttributeIndex);

	if (IsProperty (obj))
		return MetadataCache::GenerateCustomAttributeTypeCache ((((Il2CppReflectionProperty*)obj)->property)->customAttributeIndex);

	if (IsField (obj))
		return MetadataCache::GenerateCustomAttributeTypeCache ((((Il2CppReflectionField*)obj)->field)->customAttributeIndex);

	if (IsEvent (obj))
		return MetadataCache::GenerateCustomAttributeTypeCache ((((Il2CppReflectionMonoEvent*)obj)->eventInfo)->customAttributeIndex);

	if (IsParameter (obj))
		return GetCustomAttributeTypeCacheFor ((Il2CppReflectionParameter*)obj);

	if (IsAssembly (obj))
		return MetadataCache::GenerateCustomAttributeTypeCache ((((Il2CppReflectionAssembly*)obj)->assembly)->customAttributeIndex);

	Il2CppClass *klass = IsType (obj)
		? Class::FromSystemType ((Il2CppReflectionType*)obj)
		: obj->klass;

	return MetadataCache::GenerateCustomAttributeTypeCache (klass->customAttributeIndex);

}

void Reflection::Initialize ()
{
	s_AssemblyMap = new AssemblyMap ();
	s_FieldMap = new FieldMap ();
	s_PropertyMap = new PropertyMap ();
	s_EventMap = new EventMap ();
	s_MethodMap = new MethodMap ();
	s_ModuleMap = new ModuleMap ();
	s_ParametersMap = new ParametersMap ();
	s_TypeMap = new TypeMap ();
}

bool Reflection::CustomAttrsHasAttr (CustomAttributeTypeCache *ainfo, Il2CppClass *attr_klass)
{
	assert (ainfo);
	assert (attr_klass);

	int i;
	Il2CppClass *klass;

	for(i = 0; i < ainfo->count; ++i)
	{
		klass = ainfo->attributeTypes[i];

		if (Class::HasParent (klass, attr_klass) || (Class::IsInterface (attr_klass) && Class::IsAssignableFrom (attr_klass, klass)))
			return true;
	}
	return false;
}

bool Reflection::HasAttribute(Il2CppObject *obj, Il2CppClass *attribute)
{
	CustomAttributeTypeCache* attrs = GetCustomAttrsTypeInfo(obj);
	if (!attrs)
		return false;

	return CustomAttrsHasAttr(attrs, attribute);
}

bool Reflection::HasAttribute(FieldInfo *field, Il2CppClass *attribute)
{
	CustomAttributeTypeCache* attrs = MetadataCache::GenerateCustomAttributeTypeCache(field->customAttributeIndex);
	if (!attrs)
		return false;

	
	return CustomAttrsHasAttr(attrs, attribute);
}

bool Reflection::HasAttribute(const MethodInfo *method, Il2CppClass *attribute)
{
	CustomAttributeTypeCache* attrs = MetadataCache::GenerateCustomAttributeTypeCache(method->customAttributeIndex);
	if (!attrs)
		return false;

	return CustomAttrsHasAttr(attrs, attribute);
}

bool Reflection::HasAttribute(Il2CppClass *klass, Il2CppClass *attribute)
{
	CustomAttributeTypeCache* attrs = MetadataCache::GenerateCustomAttributeTypeCache(klass->customAttributeIndex);
	if (!attrs)
		return false;

	return CustomAttrsHasAttr(attrs, attribute);
}


Il2CppClass* Reflection::TypeGetHandle (Il2CppReflectionType* ref)
{
	if (!ref)
		return NULL;

	return Class::FromSystemType (ref);
}

} /* namespace vm */
} /* namespace il2cpp */
