#pragma once

#include "common.h"

struct Il2CppClass;
struct Il2CppObject;
struct Il2CppDomain;
struct Il2CppAssembly;
struct Il2CppImage;

namespace il2cpp
{
namespace debugger
{

enum IdType
{
	// Note: kIdTypeObject is used only internally, it should never
	// be sent to the client 'cause it is invalid.
	kIdTypeObject = -1,
	kIdTypeAssembly = 0,
	kIdTypeModule = 1,
	kIdTypeType = 2,
	kIdTypeMethod = 3,
	kIdTypeField = 4,
	kIdTypeDomain = 5,
	kIdTypeProperty = 6,
	kIdTypeCount
};

struct Id
{
	IdType type;
	int32_t id;
};

Id AssemblyId(const Il2CppAssembly *assembly);
Id ModuleId(const Il2CppImage *image);
Id TypeId(const Il2CppClass *type);
Id DomainId(const Il2CppDomain *domain);
Id ObjectId(const Il2CppObject *object);
Id MethodId(const MethodInfo *method);
Id FieldId(const FieldInfo *field);
Id PropertyId(const PropertyInfo *prop);
Il2CppClass *TypeFromId(Id &id);
Il2CppClass *TypeFromId(int32_t id);
Il2CppObject *ObjectFromId(Id &id);
Il2CppDomain *DomainFromId(Id &id);
Il2CppAssembly *AssemblyFromId(Id &id);
MethodInfo *MethodFromId(Id &id);
FieldInfo *FieldFromId(Id &id);
PropertyInfo *PropertyFromId(Id &id);
Il2CppImage *ModuleFromId(Id &id);

} /* namespace debugger */
} /* namespace il2cpp */
