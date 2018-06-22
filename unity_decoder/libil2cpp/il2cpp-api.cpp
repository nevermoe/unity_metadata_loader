#include "il2cpp-api.h"
#include "object-internals.h"

#include "vm/Array.h"
#include "vm/Assembly.h"
#include "vm/Class.h"
#include "vm/Domain.h"
#include "vm/Exception.h"
#include "vm/Field.h"
#include "vm/Image.h"
#include "vm/InternalCalls.h"
#include "vm/Liveness.h"
#include "vm/MemoryInformation.h"
#include "vm/Method.h"
#include "vm/Monitor.h"
#include "vm/Object.h"
#include "vm/Path.h"
#include "vm/PlatformInvoke.h"
#include "vm/Profiler.h"
#include "vm/Property.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/StackTrace.h"
#include "vm/String.h"
#include "vm/Thread.h"
#include "vm/ThreadPool.h"
#include "vm/Type.h"
#include "utils/Exception.h"
#include "utils/Logging.h"
#include "utils/Memory.h"
#include "utils/StringUtils.h"
#include "utils/Runtime.h"
#include "utils/Environment.h"

#if IL2CPP_DEBUGGER_ENABLED
    #include "il2cpp-debugger.h"
    #include "vm/Debug.h"
#endif

#include "gc/GarbageCollector.h"
#include "gc/GCHandle.h"

#include <locale.h>
#include <fstream>

using namespace il2cpp::vm;
using il2cpp::utils::Memory;
using namespace il2cpp::gc;

#if IL2CPP_API_DYNAMIC_NO_DLSYM
#include <map>

struct SymbolCompare : public std::binary_function<char*, char*, bool>
{
    bool operator()(const char* lhs, const char* rhs) const
    {
        return strcmp(lhs, rhs) < 0;
    }
};

typedef std::map<const char*, void*, SymbolCompare> SymbolTable;
static SymbolTable s_SymbolTable;

static void RegisterAPIFunction(const char* name, void* symbol)
{
    s_SymbolTable.insert(std::make_pair(name, symbol));
}

void il2cpp_api_register_symbols(void)
{
    #define DO_API(r, n, p) RegisterAPIFunction(#n, (void*)n);
    #include "il2cpp-api-functions.h"
    #undef DO_API
}

void* il2cpp_api_lookup_symbol(const char* name)
{
    SymbolTable::iterator it = s_SymbolTable.find(name);
    if (it != s_SymbolTable.end())
    {
        return it->second;
    }
    return NULL;
}

#endif // IL2CPP_API_DYNAMIC_NO_DLSYM

void il2cpp_init(const char* domain_name)
{
    // Use environment's default locale
    setlocale(LC_ALL, "");

#if IL2CPP_DEBUGGER_ENABLED
    if (!il2cpp_debugger_agent_is_initialized())
        il2cpp_debugger_agent_init();
#endif

    // NOTE(gab): the runtime_version needs to change once we
    // will support multiple runtimes.
    // For now we default to the one used by unity and don't
    // allow the callers to change it.
#if NET_4_0
    Runtime::Init(domain_name, "v4.0.30319");
#else
    Runtime::Init(domain_name, "v2.0.50727");
#endif

#if IL2CPP_DEBUGGER_ENABLED
    il2cpp_debugger_notify_vm_start();
#endif
}

void il2cpp_init_utf16(const Il2CppChar* domain_name)
{
    return il2cpp_init(il2cpp::utils::StringUtils::Utf16ToUtf8(domain_name).c_str());
}

void il2cpp_shutdown()
{
#if IL2CPP_DEBUGGER_ENABLED
    il2cpp_debugger_notify_vm_death();
#endif

    Runtime::Shutdown();

#if IL2CPP_DEBUGGER_ENABLED
    il2cpp_debugger_agent_dispose();
#endif
}

void il2cpp_set_config_dir(const char *config_path)
{
    il2cpp::vm::Runtime::SetConfigDir(config_path);
}

void il2cpp_set_data_dir(const char *data_path)
{
    il2cpp::utils::Runtime::SetDataDir(data_path);
}

void il2cpp_set_temp_dir(const char *temp_dir)
{
    il2cpp::vm::Path::SetTempPath(temp_dir);
}

void il2cpp_set_commandline_arguments(int argc, const char* const argv[], const char* basedir)
{
    il2cpp::utils::Environment::SetMainArgs(argv, argc);
}

void il2cpp_set_commandline_arguments_utf16(int argc, const Il2CppChar* const argv[], const char* basedir)
{
    il2cpp::utils::Environment::SetMainArgs(argv, argc);
}

void il2cpp_set_config_utf16(const Il2CppChar* executablePath)
{
    il2cpp::vm::Runtime::SetConfigUtf16(executablePath);
}

void il2cpp_set_config(const char* executablePath)
{
    il2cpp::vm::Runtime::SetConfig(executablePath);
}

void il2cpp_set_memory_callbacks(Il2CppMemoryCallbacks* callbacks)
{
    Memory::SetMemoryCallbacks(callbacks);
}

const Il2CppImage* il2cpp_get_corlib()
{
    return Image::GetCorlib();
}

void il2cpp_add_internal_call(const char* name, Il2CppMethodPointer method)
{
    return InternalCalls::Add(name, method);
}

Il2CppMethodPointer il2cpp_resolve_icall(const char* name)
{
    return InternalCalls::Resolve(name);
}

void* il2cpp_alloc(size_t size)
{
    return IL2CPP_MALLOC(size);
}

void il2cpp_free(void* ptr)
{
    IL2CPP_FREE(ptr);
}

// array

Il2CppClass *il2cpp_array_class_get(Il2CppClass *element_class, uint32_t rank)
{
    return Class::GetArrayClass(element_class, rank);
}

uint32_t il2cpp_array_length(Il2CppArray* array)
{
    return Array::GetLength(array);
}

uint32_t il2cpp_array_get_byte_length(Il2CppArray *array)
{
    return Array::GetByteLength(array);
}

Il2CppArray* il2cpp_array_new(Il2CppClass *elementTypeInfo, il2cpp_array_size_t length)
{
    return Array::New(elementTypeInfo, length);
}

Il2CppArray* il2cpp_array_new_specific(Il2CppClass *arrayTypeInfo, il2cpp_array_size_t length)
{
    return Array::NewSpecific(arrayTypeInfo, length);
}

Il2CppArray* il2cpp_array_new_full(Il2CppClass *array_class, il2cpp_array_size_t *lengths, il2cpp_array_size_t *lower_bounds)
{
    return Array::NewFull(array_class, lengths, lower_bounds);
}

Il2CppClass* il2cpp_bounded_array_class_get(Il2CppClass *element_class, uint32_t rank, bool bounded)
{
    return Class::GetBoundedArrayClass(element_class, rank, bounded);
}

int il2cpp_array_element_size(const Il2CppClass* klass)
{
    return Array::GetElementSize(klass);
}

// assembly
const Il2CppImage* il2cpp_assembly_get_image(const Il2CppAssembly *assembly)
{
    return Assembly::GetImage(assembly);
}

// class

const Il2CppType* il2cpp_class_enum_basetype(Il2CppClass *klass)
{
    return Class::GetEnumBaseType(klass);
}

Il2CppClass* il2cpp_class_from_system_type(Il2CppReflectionType *type)
{
    return Class::FromSystemType(type);
}

bool il2cpp_class_is_generic(const Il2CppClass *klass)
{
    return Class::IsGeneric(klass);
}

bool il2cpp_class_is_inflated(const Il2CppClass *klass)
{
    return Class::IsInflated(klass);
}

bool il2cpp_class_is_assignable_from(Il2CppClass *klass, Il2CppClass *oklass)
{
    return Class::IsAssignableFrom(klass, oklass);
}

bool il2cpp_class_is_subclass_of(Il2CppClass *klass, Il2CppClass *klassc, bool check_interfaces)
{
    return Class::IsSubclassOf(klass, klassc, check_interfaces);
}

bool il2cpp_class_has_parent(Il2CppClass *klass, Il2CppClass *klassc)
{
    return Class::HasParent(klass, klassc);
}

Il2CppClass* il2cpp_class_from_il2cpp_type(const Il2CppType* type)
{
    return Class::FromIl2CppType(type);
}

Il2CppClass* il2cpp_class_from_name(const Il2CppImage* image, const char* namespaze, const char *name)
{
    return Class::FromName(image, namespaze, name);
}

Il2CppClass* il2cpp_class_get_element_class(Il2CppClass *klass)
{
    return Class::GetElementClass(klass);
}

const EventInfo* il2cpp_class_get_events(Il2CppClass *klass, void* *iter)
{
    return Class::GetEvents(klass, iter);
}

FieldInfo* il2cpp_class_get_fields(Il2CppClass *klass, void* *iter)
{
    return Class::GetFields(klass, iter);
}

Il2CppClass* il2cpp_class_get_nested_types(Il2CppClass *klass, void* *iter)
{
    return Class::GetNestedTypes(klass, iter);
}

Il2CppClass* il2cpp_class_get_interfaces(Il2CppClass *klass, void* *iter)
{
    return Class::GetInterfaces(klass, iter);
}

const PropertyInfo* il2cpp_class_get_properties(Il2CppClass *klass, void* *iter)
{
    return Class::GetProperties(klass, iter);
}

const PropertyInfo* il2cpp_class_get_property_from_name(Il2CppClass *klass, const char *name)
{
    return Class::GetPropertyFromName(klass, name);
}

FieldInfo* il2cpp_class_get_field_from_name(Il2CppClass* klass, const char *name)
{
    return Class::GetFieldFromName(klass, name);
}

const MethodInfo* il2cpp_class_get_methods(Il2CppClass *klass, void* *iter)
{
    return Class::GetMethods(klass, iter);
}

const MethodInfo* il2cpp_class_get_method_from_name(Il2CppClass *klass, const char* name, int argsCount)
{
    return Class::GetMethodFromName(klass, name, argsCount);
}

const char* il2cpp_class_get_name(Il2CppClass *klass)
{
    return Class::GetName(klass);
}

const char* il2cpp_class_get_namespace(Il2CppClass *klass)
{
    return Class::GetNamespace(klass);
}

Il2CppClass* il2cpp_class_get_parent(Il2CppClass *klass)
{
    return Class::GetParent(klass);
}

Il2CppClass* il2cpp_class_get_declaring_type(Il2CppClass* klass)
{
    return Class::GetDeclaringType(klass);
}

int32_t il2cpp_class_instance_size(Il2CppClass *klass)
{
    return Class::GetInstanceSize(klass);
}

size_t il2cpp_class_num_fields(const Il2CppClass* klass)
{
    return Class::GetNumFields(klass);
}

bool il2cpp_class_is_valuetype(const Il2CppClass* klass)
{
    return Class::IsValuetype(klass);
}

bool il2cpp_class_is_blittable(const Il2CppClass* klass)
{
    return Class::IsBlittable(klass);
}

int32_t il2cpp_class_value_size(Il2CppClass *klass, uint32_t *align)
{
    return Class::GetValueSize(klass, align);
}

int il2cpp_class_get_flags(const Il2CppClass *klass)
{
    return Class::GetFlags(klass);
}

bool il2cpp_class_is_abstract(const Il2CppClass *klass)
{
    return Class::IsAbstract(klass);
}

bool il2cpp_class_is_interface(const Il2CppClass *klass)
{
    return Class::IsInterface(klass);
}

int il2cpp_class_array_element_size(const Il2CppClass *klass)
{
    return Class::GetArrayElementSize(klass);
}

Il2CppClass* il2cpp_class_from_type(const Il2CppType *type)
{
    return Class::FromIl2CppType(type);
}

const Il2CppType* il2cpp_class_get_type(Il2CppClass *klass)
{
    return Class::GetType(klass);
}

bool il2cpp_class_has_attribute(Il2CppClass *klass, Il2CppClass *attr_class)
{
    return Class::HasAttribute(klass, attr_class);
}

bool il2cpp_class_has_references(Il2CppClass *klass)
{
    return Class::HasReferences(klass);
}

bool il2cpp_class_is_enum(const Il2CppClass *klass)
{
    return Class::IsEnum(klass);
}

const Il2CppImage* il2cpp_class_get_image(Il2CppClass* klass)
{
    return Class::GetImage(klass);
}

const char *il2cpp_class_get_assemblyname(const Il2CppClass *klass)
{
    return Class::GetAssemblyName(klass);
}

// testing only
size_t il2cpp_class_get_bitmap_size(const Il2CppClass *klass)
{
    return Class::GetBitmapSize(klass);
}

void il2cpp_class_get_bitmap(Il2CppClass *klass, size_t* bitmap)
{
    size_t dummy = 0;
    Class::GetBitmap(klass, bitmap, dummy);
}

// stats

extern Il2CppRuntimeStats il2cpp_runtime_stats;

bool il2cpp_stats_dump_to_file(const char *path)
{
    std::fstream fs;

    fs.open(path, std::fstream::out | std::fstream::trunc);

    fs << "New object count: " << il2cpp_stats_get_value(IL2CPP_STAT_NEW_OBJECT_COUNT) << "\n";
    fs << "Method count: " << il2cpp_stats_get_value(IL2CPP_STAT_METHOD_COUNT) << "\n";
    fs << "Class static data size: " << il2cpp_stats_get_value(IL2CPP_STAT_CLASS_STATIC_DATA_SIZE) << "\n";
    fs << "Inflated method count: " << il2cpp_stats_get_value(IL2CPP_STAT_INFLATED_METHOD_COUNT) << "\n";
    fs << "Inflated type count: " << il2cpp_stats_get_value(IL2CPP_STAT_INFLATED_TYPE_COUNT) << "\n";
    fs << "Initialized class count: " << il2cpp_stats_get_value(IL2CPP_STAT_INITIALIZED_CLASS_COUNT) << "\n";
    fs << "Generic instance count: " << il2cpp_stats_get_value(IL2CPP_STAT_GENERIC_INSTANCE_COUNT) << "\n";
    fs << "Generic class count: " << il2cpp_stats_get_value(IL2CPP_STAT_GENERIC_CLASS_COUNT) << "\n";

    fs.close();

    return true;
}

uint64_t il2cpp_stats_get_value(Il2CppStat stat)
{
    switch (stat)
    {
        case IL2CPP_STAT_NEW_OBJECT_COUNT:
            return il2cpp_runtime_stats.new_object_count;

        case IL2CPP_STAT_INITIALIZED_CLASS_COUNT:
            return il2cpp_runtime_stats.initialized_class_count;

        /*case IL2CPP_STAT_GENERIC_VTABLE_COUNT:
            return il2cpp_runtime_stats.generic_vtable_count;

        case IL2CPP_STAT_USED_CLASS_COUNT:
            return il2cpp_runtime_stats.used_class_count;*/

        case IL2CPP_STAT_METHOD_COUNT:
            return il2cpp_runtime_stats.method_count;

        /*case IL2CPP_STAT_CLASS_VTABLE_SIZE:
            return il2cpp_runtime_stats.class_vtable_size;*/

        case IL2CPP_STAT_CLASS_STATIC_DATA_SIZE:
            return il2cpp_runtime_stats.class_static_data_size;

        case IL2CPP_STAT_GENERIC_INSTANCE_COUNT:
            return il2cpp_runtime_stats.generic_instance_count;

        case IL2CPP_STAT_GENERIC_CLASS_COUNT:
            return il2cpp_runtime_stats.generic_class_count;

        case IL2CPP_STAT_INFLATED_METHOD_COUNT:
            return il2cpp_runtime_stats.inflated_method_count;

        case IL2CPP_STAT_INFLATED_TYPE_COUNT:
            return il2cpp_runtime_stats.inflated_type_count;

            /*case IL2CPP_STAT_DELEGATE_CREATIONS:
                return il2cpp_runtime_stats.delegate_creations;

            case IL2CPP_STAT_MINOR_GC_COUNT:
                return il2cpp_runtime_stats.minor_gc_count;

            case IL2CPP_STAT_MAJOR_GC_COUNT:
                return il2cpp_runtime_stats.major_gc_count;

            case IL2CPP_STAT_MINOR_GC_TIME_USECS:
                return il2cpp_runtime_stats.minor_gc_time_usecs;

            case IL2CPP_STAT_MAJOR_GC_TIME_USECS:
                return il2cpp_runtime_stats.major_gc_time_usecs;*/
    }

    return 0;
}

// domain
Il2CppDomain* il2cpp_domain_get()
{
    return Domain::GetCurrent();
}

const Il2CppAssembly* il2cpp_domain_assembly_open(Il2CppDomain *domain, const char *name)
{
    return Assembly::Load(name);
}

const Il2CppAssembly** il2cpp_domain_get_assemblies(const Il2CppDomain* domain, size_t* size)
{
    il2cpp::vm::AssemblyVector* assemblies = Assembly::GetAllAssemblies();
    *size = assemblies->size();
    return &(*assemblies)[0];
}

// exception

void il2cpp_raise_exception(Il2CppException* exc)
{
    Exception::Raise(exc);
}

Il2CppException* il2cpp_exception_from_name_msg(const Il2CppImage* image, const char *name_space, const char *name, const char *msg)
{
    return Exception::FromNameMsg(image, name_space, name, msg);
}

Il2CppException* il2cpp_get_exception_argument_null(const char *arg)
{
    return Exception::GetArgumentNullException(arg);
}

void il2cpp_format_exception(const Il2CppException* ex, char* message, int message_size)
{
    strncpy(message, il2cpp::utils::Exception::FormatException(ex).c_str(), message_size);
}

void il2cpp_format_stack_trace(const Il2CppException* ex, char* output, int output_size)
{
    strncpy(output, il2cpp::utils::Exception::FormatStackTrace(ex).c_str(), output_size);
}

void il2cpp_unhandled_exception(Il2CppException* exc)
{
    Runtime::UnhandledException(exc);
}

// field

const char* il2cpp_field_get_name(FieldInfo *field)
{
    return Field::GetName(field);
}

int il2cpp_field_get_flags(FieldInfo *field)
{
    return Field::GetFlags(field);
}

Il2CppClass* il2cpp_field_get_parent(FieldInfo *field)
{
    return Field::GetParent(field);
}

size_t il2cpp_field_get_offset(FieldInfo *field)
{
    return Field::GetOffset(field);
}

const Il2CppType* il2cpp_field_get_type(FieldInfo *field)
{
    return Field::GetType(field);
}

void il2cpp_field_get_value(Il2CppObject *obj, FieldInfo *field, void *value)
{
    return Field::GetValue(obj, field, value);
}

Il2CppObject* il2cpp_field_get_value_object(FieldInfo *field, Il2CppObject *obj)
{
    return Field::GetValueObject(field, obj);
}

bool il2cpp_field_has_attribute(FieldInfo *field, Il2CppClass *attr_class)
{
    return Field::HasAttribute(field, attr_class);
}

void il2cpp_field_set_value(Il2CppObject *obj, FieldInfo *field, void *value)
{
    Field::SetValue(obj, field, value);
}

void il2cpp_field_set_value_object(Il2CppObject* objectInstance, FieldInfo* field, Il2CppObject* value)
{
    Field::SetInstanceFieldValueObject(objectInstance, field, value);
}

void il2cpp_field_static_get_value(FieldInfo *field, void *value)
{
    Field::StaticGetValue(field, value);
}

void il2cpp_field_static_set_value(FieldInfo *field, void *value)
{
    Field::StaticSetValue(field, value);
}

// gc
void il2cpp_gc_collect(int maxGenerations)
{
    GarbageCollector::Collect(maxGenerations);
}

int32_t il2cpp_gc_collect_a_little()
{
    return GarbageCollector::CollectALittle();
}

void il2cpp_gc_enable()
{
    GarbageCollector::Enable();
}

void il2cpp_gc_disable()
{
    GarbageCollector::Disable();
}

int64_t il2cpp_gc_get_used_size()
{
    return GarbageCollector::GetUsedHeapSize();
}

int64_t il2cpp_gc_get_heap_size()
{
    return GarbageCollector::GetAllocatedHeapSize();
}

// gchandle

uint32_t il2cpp_gchandle_new(Il2CppObject *obj, bool pinned)
{
    return GCHandle::New(obj, pinned);
}

uint32_t il2cpp_gchandle_new_weakref(Il2CppObject *obj, bool track_resurrection)
{
    return GCHandle::NewWeakref(obj, track_resurrection);
}

Il2CppObject* il2cpp_gchandle_get_target(uint32_t gchandle)
{
    return GCHandle::GetTarget(gchandle);
}

void il2cpp_gchandle_free(uint32_t gchandle)
{
    GCHandle::Free(gchandle);
}

// liveness

void* il2cpp_unity_liveness_calculation_begin(Il2CppClass* filter, int max_object_count, il2cpp_register_object_callback callback, void* userdata, il2cpp_WorldChangedCallback onWorldStarted, il2cpp_WorldChangedCallback onWorldStopped)
{
    return Liveness::Begin(filter, max_object_count, callback, userdata, onWorldStarted, onWorldStopped);
}

void il2cpp_unity_liveness_calculation_end(void* state)
{
    Liveness::End(state);
}

void il2cpp_unity_liveness_calculation_from_root(Il2CppObject* root, void* state)
{
    Liveness::FromRoot(root, state);
}

void il2cpp_unity_liveness_calculation_from_statics(void* state)
{
    Liveness::FromStatics(state);
}

// method

const Il2CppType* il2cpp_method_get_return_type(const MethodInfo* method)
{
    return Method::GetReturnType(method);
}

Il2CppReflectionMethod* il2cpp_method_get_object(const MethodInfo *method, Il2CppClass *refclass)
{
    return Reflection::GetMethodObject(method, refclass);
}

const char* il2cpp_method_get_name(const MethodInfo *method)
{
    return Method::GetName(method);
}

bool il2cpp_method_is_generic(const MethodInfo *method)
{
    return Method::IsGeneric(method);
}

bool il2cpp_method_is_inflated(const MethodInfo *method)
{
    return Method::IsInflated(method);
}

bool il2cpp_method_is_instance(const MethodInfo *method)
{
    return Method::IsInstance(method);
}

uint32_t il2cpp_method_get_param_count(const MethodInfo *method)
{
    return Method::GetParamCount(method);
}

const Il2CppType* il2cpp_method_get_param(const MethodInfo *method, uint32_t index)
{
    return Method::GetParam(method, index);
}

Il2CppClass* il2cpp_method_get_class(const MethodInfo *method)
{
    return Method::GetClass(method);
}

bool il2cpp_method_has_attribute(const MethodInfo *method, Il2CppClass *attr_class)
{
    return Method::HasAttribute(method, attr_class);
}

Il2CppClass* il2cpp_method_get_declaring_type(const MethodInfo* method)
{
    return Method::GetDeclaringType(method);
}

uint32_t il2cpp_method_get_flags(const MethodInfo *method, uint32_t *iflags)
{
    if (iflags != 0)
        *iflags = Method::GetImplementationFlags(method);

    return Method::GetFlags(method);
}

uint32_t il2cpp_method_get_token(const MethodInfo *method)
{
    return Method::GetToken(method);
}

const char *il2cpp_method_get_param_name(const MethodInfo *method, uint32_t index)
{
    return Method::GetParamName(method, index);
}

// profiler

#if IL2CPP_ENABLE_PROFILER

void il2cpp_profiler_install(Il2CppProfiler *prof, Il2CppProfileFunc shutdown_callback)
{
    Profiler::Install(prof, shutdown_callback);
}

void il2cpp_profiler_set_events(Il2CppProfileFlags events)
{
    Profiler::SetEvents(events);
}

void il2cpp_profiler_install_enter_leave(Il2CppProfileMethodFunc enter, Il2CppProfileMethodFunc fleave)
{
    Profiler::InstallEnterLeave(enter, fleave);
}

void il2cpp_profiler_install_allocation(Il2CppProfileAllocFunc callback)
{
    Profiler::InstallAllocation(callback);
}

void il2cpp_profiler_install_gc(Il2CppProfileGCFunc callback, Il2CppProfileGCResizeFunc heap_resize_callback)
{
    Profiler::InstallGC(callback, heap_resize_callback);
}

#endif

// property

const char* il2cpp_property_get_name(PropertyInfo *prop)
{
    return Property::GetName(prop);
}

const MethodInfo* il2cpp_property_get_get_method(PropertyInfo *prop)
{
    return Property::GetGetMethod(prop);
}

const MethodInfo* il2cpp_property_get_set_method(PropertyInfo *prop)
{
    return Property::GetSetMethod(prop);
}

Il2CppClass* il2cpp_property_get_parent(PropertyInfo *prop)
{
    return Property::GetParent(prop);
}

uint32_t il2cpp_property_get_flags(PropertyInfo *prop)
{
    return Property::GetFlags(prop);
}

// object

Il2CppClass* il2cpp_object_get_class(Il2CppObject* obj)
{
    return Object::GetClass(obj);
}

uint32_t il2cpp_object_get_size(Il2CppObject* obj)
{
    return Object::GetSize(obj);
}

const MethodInfo* il2cpp_object_get_virtual_method(Il2CppObject *obj, const MethodInfo *method)
{
    return Object::GetVirtualMethod(obj, method);
}

Il2CppObject* il2cpp_object_new(const Il2CppClass *klass)
{
    try
    {
        return Object::New(const_cast<Il2CppClass*>(klass));
    }
    catch (const Il2CppExceptionWrapper&)
    {
        // If a static constructor throws, that exception will occur here.
        // We don't want that to escape across the embedding API.
        return NULL;
    }
}

void* il2cpp_object_unbox(Il2CppObject* obj)
{
    return Object::Unbox(obj);
}

Il2CppObject* il2cpp_value_box(Il2CppClass *klass, void* data)
{
    return Object::Box(klass, data);
}

// monitor
void il2cpp_monitor_enter(Il2CppObject* obj)
{
    Monitor::Enter(obj);
}

bool il2cpp_monitor_try_enter(Il2CppObject* obj, uint32_t timeout)
{
    return Monitor::TryEnter(obj, timeout);
}

void il2cpp_monitor_exit(Il2CppObject* obj)
{
    Monitor::Exit(obj);
}

void il2cpp_monitor_pulse(Il2CppObject* obj)
{
    Monitor::Pulse(obj);
}

void il2cpp_monitor_pulse_all(Il2CppObject* obj)
{
    Monitor::PulseAll(obj);
}

void il2cpp_monitor_wait(Il2CppObject* obj)
{
    Monitor::Wait(obj);
}

bool il2cpp_monitor_try_wait(Il2CppObject* obj, uint32_t timeout)
{
    return Monitor::TryWait(obj, timeout);
}

// runtime

Il2CppObject* il2cpp_runtime_invoke_convert_args(const MethodInfo *method, void *obj, Il2CppObject **params, int paramCount, Il2CppException **exc)
{
    // Our embedding API has historically taken pointers to unboxed value types, rather than Il2CppObjects.
    // However, with the introduction of adjustor thunks, our invokees expect us to pass them Il2CppObject*, or at least something that
    // ressembles boxed value type. Since it's not going to access any of the Il2CppObject* fields,
    // it's fine to just subtract sizeof(Il2CppObject) from obj pointer
    if (method->declaring_type->valuetype)
        obj = static_cast<Il2CppObject*>(obj) - 1;

    return Runtime::InvokeConvertArgs(method, obj, params, paramCount, exc);
}

Il2CppObject* il2cpp_runtime_invoke(const MethodInfo *method,
    void *obj, void **params, Il2CppException **exc)
{
    // Our embedding API has historically taken pointers to unboxed value types, rather than Il2CppObjects.
    // However, with the introduction of adjustor thunks, our invokees expect us to pass them Il2CppObject*, or at least something that
    // ressembles boxed value type. Since it's not going to access any of the Il2CppObject* fields,
    // it's fine to just subtract sizeof(Il2CppObject) from obj pointer
    if (method->declaring_type->valuetype)
        obj = static_cast<Il2CppObject*>(obj) - 1;

    return Runtime::Invoke(method, obj, params, exc);
}

void il2cpp_runtime_class_init(Il2CppClass* klass)
{
    return Runtime::ClassInit(klass);
}

void il2cpp_runtime_object_init(Il2CppObject *obj)
{
    Runtime::ObjectInit(obj);
}

void il2cpp_runtime_object_init_exception(Il2CppObject *obj, Il2CppException **exc)
{
    Runtime::ObjectInitException(obj, exc);
}

void il2cpp_runtime_unhandled_exception_policy_set(Il2CppRuntimeUnhandledExceptionPolicy value)
{
    Runtime::SetUnhandledExceptionPolicy(value);
}

// string

int32_t il2cpp_string_length(Il2CppString* str)
{
    return il2cpp::utils::StringUtils::GetLength(str);
}

Il2CppChar* il2cpp_string_chars(Il2CppString* str)
{
    return il2cpp::utils::StringUtils::GetChars(str);
}

// Same as il2cpp_string_new_wrapper, because other normally takes a domain
Il2CppString* il2cpp_string_new(const char* str)
{
    return String::New(str);
}

Il2CppString* il2cpp_string_new_wrapper(const char* str)
{
    return String::NewWrapper(str);
}

Il2CppString* il2cpp_string_new_utf16(const Il2CppChar *text, int32_t len)
{
    return String::NewUtf16(text, len);
}

Il2CppString* il2cpp_string_new_len(const char* str, uint32_t length)
{
    return String::NewLen(str, length);
}

Il2CppString* il2cpp_string_intern(Il2CppString* str)
{
    return String::Intern(str);
}

Il2CppString* il2cpp_string_is_interned(Il2CppString* str)
{
    return String::IsInterned(str);
}

// thread

char *il2cpp_thread_get_name(Il2CppThread *thread, uint32_t *len)
{
    return Thread::GetName(len);
}

Il2CppThread *il2cpp_thread_current()
{
    return Thread::Current();
}

Il2CppThread *il2cpp_thread_attach(Il2CppDomain *domain)
{
    return Thread::Attach(domain);
}

void il2cpp_thread_detach(Il2CppThread *thread)
{
    Thread::Detach(thread);
}

Il2CppThread **il2cpp_thread_get_all_attached_threads(size_t *size)
{
    return Thread::GetAllAttachedThreads(*size);
}

bool il2cpp_is_vm_thread(Il2CppThread *thread)
{
    return Thread::IsVmThread(thread);
}

// stacktrace

void il2cpp_current_thread_walk_frame_stack(Il2CppFrameWalkFunc func, void* user_data)
{
    StackTrace::WalkFrameStack(func, user_data);
}

void il2cpp_thread_walk_frame_stack(Il2CppThread *thread, Il2CppFrameWalkFunc func, void *user_data)
{
    return StackTrace::WalkThreadFrameStack(thread, func, user_data);
}

bool il2cpp_current_thread_get_top_frame(Il2CppStackFrameInfo& frame)
{
    return StackTrace::GetTopStackFrame(frame);
}

bool il2cpp_thread_get_top_frame(Il2CppThread* thread, Il2CppStackFrameInfo& frame)
{
    return StackTrace::GetThreadTopStackFrame(thread, frame);
}

bool il2cpp_current_thread_get_frame_at(int32_t offset, Il2CppStackFrameInfo& frame)
{
    return StackTrace::GetStackFrameAt(offset, frame);
}

bool il2cpp_thread_get_frame_at(Il2CppThread* thread, int32_t offset, Il2CppStackFrameInfo& frame)
{
    return StackTrace::GetThreadStackFrameAt(thread, offset, frame);
}

int32_t il2cpp_current_thread_get_stack_depth()
{
    return static_cast<int32_t>(StackTrace::GetStackDepth());
}

int32_t il2cpp_thread_get_stack_depth(Il2CppThread *thread)
{
    return StackTrace::GetThreadStackDepth(thread);
}

// type

Il2CppObject* il2cpp_type_get_object(const Il2CppType *type)
{
    return (Il2CppObject*)Reflection::GetTypeObject(type);
}

int il2cpp_type_get_type(const Il2CppType *type)
{
    return Type::GetType(type);
}

Il2CppClass* il2cpp_type_get_class_or_element_class(const Il2CppType *type)
{
    return Type::GetClassOrElementClass(type);
}

char* il2cpp_type_get_name(const Il2CppType *type)
{
    std::string name = Type::GetName(type, IL2CPP_TYPE_NAME_FORMAT_IL);
    char* buffer = static_cast<char*>(il2cpp_alloc(name.length() + 1));
    memcpy(buffer, name.c_str(), name.length() + 1);

    return buffer;
}

// image

const Il2CppAssembly* il2cpp_image_get_assembly(const Il2CppImage *image)
{
    return Image::GetAssembly(image);
}

const char* il2cpp_image_get_name(const Il2CppImage *image)
{
    return Image::GetName(image);
}

const char* il2cpp_image_get_filename(const Il2CppImage *image)
{
    return Image::GetFileName(image);
}

const MethodInfo* il2cpp_image_get_entry_point(const Il2CppImage *image)
{
    return Image::GetEntryPoint(image);
}

Il2CppManagedMemorySnapshot* il2cpp_capture_memory_snapshot()
{
    return MemoryInformation::CaptureManagedMemorySnapshot();
}

void il2cpp_free_captured_memory_snapshot(Il2CppManagedMemorySnapshot* snapshot)
{
    MemoryInformation::FreeCapturedManagedMemorySnapshot(snapshot);
}

void il2cpp_set_find_plugin_callback(Il2CppSetFindPlugInCallback method)
{
    il2cpp::vm::PlatformInvoke::SetFindPluginCallback(method);
}

// Logging

void il2cpp_register_log_callback(Il2CppLogCallback method)
{
    il2cpp::utils::Logging::SetLogCallback(method);
}

#if IL2CPP_DEBUGGER_ENABLED
// debug
const Il2CppDebugTypeInfo* il2cpp_debug_get_class_info(const Il2CppClass *klass)
{
    return Class::GetDebugInfo(klass);
}

const Il2CppDebugDocument* il2cpp_debug_class_get_document(const Il2CppDebugTypeInfo* info)
{
    return Debug::GetDocument(info);
}

const char* il2cpp_debug_document_get_filename(const Il2CppDebugDocument* document)
{
    return Debug::DocumentGetFilename(document);
}

const char* il2cpp_debug_document_get_directory(const Il2CppDebugDocument* document)
{
    return Debug::DocumentGetDirectory(document);
}

const Il2CppDebugMethodInfo* il2cpp_debug_get_method_info(const MethodInfo *method)
{
    return Method::GetDebugInfo(method);
}

const Il2CppDebugDocument* il2cpp_debug_method_get_document(const Il2CppDebugMethodInfo* info)
{
    return Debug::GetDocument(info);
}

const int32_t* il2cpp_debug_method_get_offset_table(const Il2CppDebugMethodInfo* info)
{
    return Debug::GetOffsetTable(info);
}

size_t il2cpp_debug_method_get_code_size(const Il2CppDebugMethodInfo* info)
{
    return Debug::GetCodeSize(info);
}

void il2cpp_debug_update_frame_il_offset(int32_t il_offset)
{
    Debug::UpdateFrameIlOffset(il_offset);
}

const Il2CppDebugLocalsInfo **il2cpp_debug_method_get_locals_info(const Il2CppDebugMethodInfo* info)
{
    return Debug::GetLocalsInfo(info);
}

const Il2CppClass *il2cpp_debug_local_get_type(const Il2CppDebugLocalsInfo *info)
{
    return Debug::GetType(info);
}

const char *il2cpp_debug_local_get_name(const Il2CppDebugLocalsInfo *info)
{
    return Debug::GetName(info);
}

uint32_t il2cpp_debug_local_get_start_offset(const Il2CppDebugLocalsInfo *info)
{
    return Debug::GetStartOffset(info);
}

uint32_t il2cpp_debug_local_get_end_offset(const Il2CppDebugLocalsInfo *info)
{
    return Debug::GetEndOffset(info);
}

Il2CppObject *il2cpp_debug_method_get_param_value(const Il2CppStackFrameInfo *info, uint32_t position)
{
    return Debug::GetParamValue(info, position);
}

Il2CppObject *il2cpp_debug_frame_get_local_value(const Il2CppStackFrameInfo *info, uint32_t position)
{
    return Debug::GetLocalValue(info, position);
}

void *il2cpp_debug_method_get_breakpoint_data_at(const Il2CppDebugMethodInfo* info, int64_t uid, int32_t offset)
{
    return Debug::GetBreakpointDataAt(info, uid, offset);
}

void il2cpp_debug_method_set_breakpoint_data_at(const Il2CppDebugMethodInfo* info, uint64_t location, void *data)
{
    Debug::SetBreakpointDataAt(info, location, data);
}

void il2cpp_debug_method_clear_breakpoint_data(const Il2CppDebugMethodInfo* info)
{
    Debug::ClearBreakpointData(info);
}

void il2cpp_debug_method_clear_breakpoint_data_at(const Il2CppDebugMethodInfo* info, uint64_t location)
{
    Debug::ClearBreakpointDataAt(info, location);
}

#endif
