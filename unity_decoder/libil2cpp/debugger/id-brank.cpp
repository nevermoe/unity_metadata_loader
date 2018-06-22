#if IL2CPP_DEBUGGER_ENABLED

#include "id-bank.h"

#include <map>

#include "../os/Atomic.h"

using namespace il2cpp::debugger;

// Mono stores the ID map per domain. We don't support domains, so we can store it globally
static std::map<IdType, std::map<uintptr_t, int32_t> > gTypesMap;

static int32_t get_or_create_id_for(IdType type, const void *ptr)
{
    if (ptr == 0)
        return 0;

    // Note: we don't use GCHandles here as long as `ptr` is always supposed to point to
    // memory statically allocated or managed directly by il2cpp (like Il2CppClass*, Il2CppType*, etc).

    if (gTypesMap.find(type) == gTypesMap.end())
    {
        gTypesMap[type] = std::map<uintptr_t, int32_t>();
    }

    const uintptr_t value_ptr = (uintptr_t)ptr;
    std::map<uintptr_t, int32_t> &type_map = gTypesMap[type];
    std::map<uintptr_t, int32_t>::iterator it = type_map.find(value_ptr);

    if (it == type_map.end())
    {
        type_map[value_ptr] = type_map.size() + 1;
    }

    return type_map[value_ptr];
}

static void *get_ptr_from_id(IdType type, int32_t id)
{
    if (id == 0)
        return 0;

    std::map<IdType, std::map<uintptr_t, int32_t> >::const_iterator itt = gTypesMap.find(type);
    if (itt == gTypesMap.end())
        return 0;

    const std::map<uintptr_t, int32_t> &type_map = itt->second;
    std::map<uintptr_t, int32_t>::const_iterator it = type_map.begin();

    // Slooooooow
    while (it != type_map.end())
    {
        if (it->second == id)
            return (void*)it->first;

        ++it;
    }

    return 0;
}

typedef uintptr_t GCHandle;

struct ObjectRef
{
    uint64_t id;
    GCHandle gc_handle; // This will be a week ref GC handle to the Object
};

static int32_t obj_reference_id = 0;
static std::map<uint32_t, ObjectRef> gIdToRefMap;
static std::map<uintptr_t, ObjectRef> gObjToRefMap;

// TODO: these il2cpp_gc_* functions need to be replaced with the right implementations later
static GCHandle il2cpp_gc_handle_new_weakref(const Il2CppObject *object, bool)
{
    return (uintptr_t)object;
}

static bool il2cpp_gc_weak_ref_is_alive(GCHandle gc_handle)
{
    return true;
}

static int32_t get_or_create_object_id(const Il2CppObject *object)
{
    if (object == 0)
        return 0;

    const uintptr_t uid = (uintptr_t)(~((uintptr_t)object));
    std::map<uintptr_t, ObjectRef>::iterator it = gObjToRefMap.find(uid);
    if (it != gObjToRefMap.end() && il2cpp_gc_weak_ref_is_alive(it->second.gc_handle))
        return it->second.id;

    ObjectRef ref;
    ref.id = il2cpp::os::Atomic::Increment(&obj_reference_id);
    ref.gc_handle = il2cpp_gc_handle_new_weakref(object, false);

    gObjToRefMap[uid] = ref;
    gIdToRefMap[ref.id] = ref;

    return ref.id;
}

static Il2CppObject *get_object_from_id(int32_t id)
{
    if (id == 0)
        return 0;

    std::map<uint32_t, ObjectRef>::iterator it = gIdToRefMap.find(id);
    if (it == gIdToRefMap.end())
    {
        LOG("Invalid object ID");
        return 0;
    }

    return (Il2CppObject*)it->second.gc_handle;
}

namespace il2cpp
{
namespace debugger
{
    Id TypeId(const Il2CppClass *type)
    {
        Id id;

        id.type = kIdTypeType;
        id.id = get_or_create_id_for(id.type, type);

        return id;
    }

    Id AssemblyId(const Il2CppAssembly *assembly)
    {
        Id id;

        id.type = kIdTypeAssembly;
        id.id = get_or_create_id_for(id.type, assembly);

        return id;
    }

    Id ModuleId(const Il2CppImage *image)
    {
        Id id;

        id.type = kIdTypeModule;
        id.id = get_or_create_id_for(id.type, image);

        return id;
    }

    Id DomainId(const Il2CppDomain *domain)
    {
        Id id;

        id.type = kIdTypeDomain;
        id.id = get_or_create_id_for(id.type, domain);

        return id;
    }

    Id ObjectId(const Il2CppObject *object)
    {
        Id id;

        id.type = kIdTypeObject;
        id.id = get_or_create_object_id(object);

        return id;
    }

    Id MethodId(const MethodInfo *method)
    {
        Id id;

        id.type = kIdTypeMethod;
        id.id = get_or_create_id_for(id.type, method);

        return id;
    }

    Id FieldId(const FieldInfo *field)
    {
        Id id;

        id.type = kIdTypeField;
        id.id = get_or_create_id_for(id.type, field);

        return id;
    }

    Id PropertyId(const PropertyInfo *prop)
    {
        Id id;

        id.type = kIdTypeProperty;
        id.id = get_or_create_id_for(id.type, prop);

        return id;
    }

    Il2CppClass *TypeFromId(int32_t id)
    {
        return (Il2CppClass*)get_ptr_from_id(kIdTypeType, id);
    }

    Il2CppClass *TypeFromId(Id &id)
    {
        IL2CPP_ASSERT(id.type == kIdTypeType && "TypeFromId can only be used on kIdTypeType ids.");

        return TypeFromId(id.id);
    }

    Il2CppObject *ObjectFromId(Id &id)
    {
        IL2CPP_ASSERT(id.type == kIdTypeObject && "ObjectFromId can only be used on kIdTypeObject ids.");

        return get_object_from_id(id.id);
    }

    Il2CppDomain *DomainFromId(Id &id)
    {
        IL2CPP_ASSERT(id.type == kIdTypeDomain && "DomainFromId can only be used on kIdTypeDomain ids.");

        return (Il2CppDomain*)get_ptr_from_id(id.type, id.id);
    }

    Il2CppAssembly *AssemblyFromId(Id &id)
    {
        IL2CPP_ASSERT(id.type == kIdTypeAssembly && "AssemblyFromId can only be used on kIdTypeAssembly ids.");

        return (Il2CppAssembly*)get_ptr_from_id(id.type, id.id);
    }

    Il2CppImage *ModuleFromId(Id &id)
    {
        IL2CPP_ASSERT(id.type == kIdTypeModule && "ModuleFromId can only be used on kIdTypeModule ids.");

        return (Il2CppImage*)get_ptr_from_id(id.type, id.id);
    }

    MethodInfo *MethodFromId(Id &id)
    {
        IL2CPP_ASSERT(id.type == kIdTypeMethod && "MethodFromId can only be used on kIdTypeMethod ids.");

        return (MethodInfo*)get_ptr_from_id(id.type, id.id);
    }

    FieldInfo *FieldFromId(Id &id)
    {
        IL2CPP_ASSERT(id.type == kIdTypeField && "FieldFromId can only be used on kIdTypeField ids.");

        return (FieldInfo*)get_ptr_from_id(id.type, id.id);
    }

    PropertyInfo *PropertyFromId(Id &id)
    {
        IL2CPP_ASSERT(id.type == kIdTypeProperty && "PropertyFromId can only be used on kIdTypeProperty ids.");

        return (PropertyInfo*)get_ptr_from_id(id.type, id.id);
    }
} /* namespace debugger */
} /* namespace il2cpp */

#endif
