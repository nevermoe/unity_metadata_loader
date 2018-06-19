#include "il2cpp-config.h"
#include "os/Environment.h"
#include "os/File.h"
#include "os/Image.h"
#include "os/Initialize.h"
#include "os/LibraryLoader.h"
#include "os/Locale.h"
#include "os/MemoryMappedFile.h"
#include "os/Mutex.h"
#include "os/Path.h"
#include "os/Thread.h"
#include "os/Socket.h"
#include "vm/Array.h"
#include "vm/Assembly.h"
#include "vm/Class.h"
#include "vm/Domain.h"
#include "vm/Environment.h"
#include "vm/Exception.h"
#include "vm/Field.h"
#include "vm/Image.h"
#include "vm/LastError.h"
#include "vm/MetadataAlloc.h"
#include "vm/MetadataCache.h"
#include "vm/MetadataLock.h"
#include "vm/Method.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/Thread.h"
#include "vm/ThreadPool.h"
#include "vm/Type.h"
#include "vm/String.h"
#include "vm/Object.h"
#include <cassert>
#include <string>
#include <map>
#include "class-internals.h"
#include "object-internals.h"
#include "tabledefs.h"
#include "gc/GarbageCollector.h"
#include "vm/InternalCalls.h"
#include "utils/Collections.h"
#include "utils/Memory.h"
#include "utils/RegisterRuntimeInitializeAndCleanup.h"
#include "utils/StringUtils.h"
#include "utils/PathUtils.h"

#if IL2CPP_DEBUGGER_ENABLED
	#include "il2cpp-debugger.h"
#endif

using il2cpp::metadata::GenericMethod;
using il2cpp::utils::StringUtils;

Il2CppIntPtr Il2CppIntPtr::Zero;
Il2CppDefaults il2cpp_defaults;
bool g_il2cpp_is_fully_initialized = false;
namespace il2cpp
{
namespace vm
{

il2cpp::os::FastMutex g_MetadataLock;

static std::string s_ConfigDir;
static std::string s_DataDir;
static std::string s_DataDirFallback;
static const char *s_FrameworkVersion = 0;
static const char *s_BundledMachineConfig = 0;
static Il2CppRuntimeUnhandledExceptionPolicy s_UnhandledExceptionPolicy = IL2CPP_UNHANDLED_POLICY_CURRENT;

#define DEFAULTS_INIT(field,ns,n) do { il2cpp_defaults.field = Class::FromName (il2cpp_defaults.corlib, ns, n); \
	assert(il2cpp_defaults.field); } while (0)

#define DEFAULTS_INIT_TYPE(field, ns, n, nativetype) do { DEFAULTS_INIT(field, ns, n); \
	assert(il2cpp_defaults.field->instance_size == sizeof(nativetype) + (il2cpp_defaults.field->valuetype ? sizeof(Il2CppObject) : 0)); } while (0)

char* basepath(const char* path)
{
	std::string original_path(path);
	size_t position_of_last_separator = original_path.find_last_of(IL2CPP_DIR_SEPARATOR);

	return il2cpp::utils::StringUtils::StringDuplicate(original_path.substr(position_of_last_separator + 1).c_str());
}

static const char *framework_version_for (const char *runtime_version)
{
	assert (runtime_version && "Invalid runtime version");
	assert ((strstr (runtime_version, "v2.0") == runtime_version) && "Invalid runtime version");

	return "2.0";
}

static void SanityChecks ()
{
#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
	assert (ALIGN_OF (int64_t) == 8);
#endif
}

void Runtime::Init(const char* filename, const char *runtime_version)
{
	SanityChecks ();

	os::Initialize();
	os::Locale::Initialize();
	MetadataAllocInitialize ();

	s_FrameworkVersion = framework_version_for (runtime_version);

	os::Thread::Init ();

	il2cpp::utils::RegisterRuntimeInitializeAndCleanup::ExecuteInitializations();

	MetadataCache::Initialize ();
	Assembly::Initialize ();
	gc::GarbageCollector::Initialize ();

	// Thread needs GC initialized
	Thread::Initialize ();

	// Reflection needs GC initialized
	Reflection::Initialize ();

	memset(&il2cpp_defaults, 0, sizeof(Il2CppDefaults));

	const Il2CppAssembly* assembly = Assembly::Load ("mscorlib.dll");

	// It is not possible to use DEFAULTS_INIT_TYPE for managed types for which we have a native struct, if the
	// native struct does not map the complete managed type.
	// Which is the case for: Il2CppThread, Il2CppAppDomain, Il2CppCultureInfo, Il2CppReflectionProperty,
	// Il2CppDateTimeFormatInfo, Il2CppNumberFormatInfo

	il2cpp_defaults.corlib = Assembly::GetImage (assembly);
	DEFAULTS_INIT(object_class, "System", "Object");
	DEFAULTS_INIT(void_class, "System", "Void");
	DEFAULTS_INIT_TYPE(boolean_class, "System", "Boolean", bool);
	DEFAULTS_INIT_TYPE(byte_class, "System", "Byte", uint8_t);
	DEFAULTS_INIT_TYPE(sbyte_class, "System", "SByte", int8_t);
	DEFAULTS_INIT_TYPE(int16_class, "System", "Int16", int16_t);
	DEFAULTS_INIT_TYPE(uint16_class, "System", "UInt16", uint16_t);
	DEFAULTS_INIT_TYPE(int32_class, "System", "Int32", int32_t);
	DEFAULTS_INIT_TYPE(uint32_class, "System", "UInt32", uint32_t);
	DEFAULTS_INIT(uint_class, "System", "UIntPtr");
	DEFAULTS_INIT_TYPE(int_class, "System", "IntPtr", Il2CppIntPtr);
	DEFAULTS_INIT_TYPE(int64_class, "System", "Int64", int64_t);
	DEFAULTS_INIT_TYPE(uint64_class, "System", "UInt64", uint64_t);
	DEFAULTS_INIT_TYPE(single_class, "System", "Single", float);
	DEFAULTS_INIT_TYPE(double_class, "System", "Double", double);
	DEFAULTS_INIT_TYPE(char_class, "System", "Char", Il2CppChar);
	DEFAULTS_INIT(string_class, "System", "String");
	DEFAULTS_INIT(enum_class, "System", "Enum");
	DEFAULTS_INIT(array_class, "System", "Array");
	DEFAULTS_INIT_TYPE(delegate_class, "System", "Delegate", Il2CppDelegate);
	DEFAULTS_INIT(multicastdelegate_class, "System", "MulticastDelegate");
	DEFAULTS_INIT(asyncresult_class, "System.Runtime.Remoting.Messaging", "AsyncResult");
	DEFAULTS_INIT_TYPE(async_call_class, "System", "MonoAsyncCall", Il2CppAsyncCall);
	DEFAULTS_INIT(manualresetevent_class, "System.Threading", "ManualResetEvent");
	//DEFAULTS_INIT(typehandle_class, "System", "RuntimeTypeHandle");
	//DEFAULTS_INIT(methodhandle_class, "System", "RuntimeMethodHandle");
	//DEFAULTS_INIT(fieldhandle_class, "System", "RuntimeFieldHandle");
	DEFAULTS_INIT(systemtype_class, "System", "Type");
	DEFAULTS_INIT_TYPE(monotype_class, "System", "MonoType", Il2CppReflectionMonoType);
	//DEFAULTS_INIT(exception_class, "System", "Exception");
	//DEFAULTS_INIT(threadabortexcepXtion_class, "System.Threading", "ThreadAbortException");
	DEFAULTS_INIT(thread_class, "System.Threading", "Thread");
	DEFAULTS_INIT(appdomain_class, "System", "AppDomain");
	DEFAULTS_INIT(appdomain_setup_class, "System", "AppDomainSetup");
	DEFAULTS_INIT(field_info_class, "System.Reflection", "FieldInfo");
	DEFAULTS_INIT(method_info_class, "System.Reflection", "MethodInfo");
	DEFAULTS_INIT(property_info_class, "System.Reflection", "PropertyInfo");
	DEFAULTS_INIT_TYPE(event_info_class, "System.Reflection", "EventInfo", Il2CppReflectionEvent);
	DEFAULTS_INIT_TYPE(mono_event_info_class, "System.Reflection", "MonoEventInfo", Il2CppReflectionMonoEventInfo);
	DEFAULTS_INIT_TYPE(stringbuilder_class, "System.Text", "StringBuilder", Il2CppStringBuilder);
	DEFAULTS_INIT_TYPE(stack_frame_class, "System.Diagnostics", "StackFrame", Il2CppStackFrame);
	DEFAULTS_INIT(stack_trace_class, "System.Diagnostics", "StackTrace");
	DEFAULTS_INIT_TYPE(typed_reference_class, "System", "TypedReference", Il2CppTypedRef);
	DEFAULTS_INIT(generic_ilist_class, "System.Collections.Generic", "IList`1");
	DEFAULTS_INIT(generic_icollection_class, "System.Collections.Generic", "ICollection`1");
	DEFAULTS_INIT(generic_ienumerable_class, "System.Collections.Generic", "IEnumerable`1");
	DEFAULTS_INIT(generic_nullable_class, "System", "Nullable`1");
	DEFAULTS_INIT(version, "System", "Version");
	DEFAULTS_INIT(culture_info, "System.Globalization", "CultureInfo");
	DEFAULTS_INIT_TYPE(assembly_class, "System.Reflection", "Assembly", Il2CppReflectionAssembly);
	DEFAULTS_INIT_TYPE(assembly_name_class, "System.Reflection", "AssemblyName", Il2CppReflectionAssemblyName);
	DEFAULTS_INIT_TYPE(enum_info_class, "System", "MonoEnumInfo", Il2CppEnumInfo);
	DEFAULTS_INIT_TYPE(mono_field_class, "System.Reflection", "MonoField", Il2CppReflectionField);
	DEFAULTS_INIT_TYPE(mono_method_class, "System.Reflection", "MonoMethod", Il2CppReflectionMethod);
	DEFAULTS_INIT_TYPE(mono_method_info_class, "System.Reflection", "MonoMethodInfo", Il2CppMethodInfo);
	DEFAULTS_INIT_TYPE(mono_property_info_class, "System.Reflection", "MonoPropertyInfo", Il2CppPropertyInfo);
	DEFAULTS_INIT_TYPE(parameter_info_class, "System.Reflection", "ParameterInfo", Il2CppReflectionParameter);
	DEFAULTS_INIT_TYPE(module_class, "System.Reflection", "Module", Il2CppReflectionModule);
	DEFAULTS_INIT_TYPE(marshal_class, "System.Reflection.Emit", "UnmanagedMarshal", Il2CppReflectionMarshal);
	DEFAULTS_INIT_TYPE(pointer_class, "System.Reflection", "Pointer", Il2CppReflectionPointer);
	DEFAULTS_INIT_TYPE(exception_class, "System", "Exception", Il2CppException);
	DEFAULTS_INIT_TYPE(system_exception_class, "System", "SystemException", Il2CppSystemException);
	DEFAULTS_INIT_TYPE(argument_exception_class, "System", "ArgumentException", Il2CppArgumentException);
	DEFAULTS_INIT_TYPE(marshalbyrefobject_class, "System", "MarshalByRefObject", Il2CppMarshalByRefObject);
	DEFAULTS_INIT_TYPE(il2cpp_com_object_class, "System", "__Il2CppComObject", Il2CppComObject);
	DEFAULTS_INIT_TYPE(wait_handle_class, "System.Threading", "WaitHandle", Il2CppWaitHandle);
	DEFAULTS_INIT_TYPE(safe_handle_class, "System.Runtime.InteropServices", "SafeHandle", Il2CppSafeHandle);
	DEFAULTS_INIT_TYPE(sort_key_class, "System.Globalization", "SortKey", Il2CppSortKey);
	DEFAULTS_INIT(dbnull_class, "System", "DBNull");
	DEFAULTS_INIT_TYPE(error_wrapper_class, "System.Runtime.InteropServices", "ErrorWrapper", Il2CppErrorWrapper);
	DEFAULTS_INIT(missing_class, "System.Reflection", "Missing");
	DEFAULTS_INIT(customattribute_data_class, "System.Reflection", "CustomAttributeData");
	DEFAULTS_INIT(value_type_class, "System", "ValueType");

	Class::Init (il2cpp_defaults.string_class);

	Il2CppDomain* domain = Domain::GetCurrent ();

	Thread::Attach (domain);

	Il2CppObject* setup = Object::NewPinned (il2cpp_defaults.appdomain_setup_class);

	Il2CppAppDomain* ad = (Il2CppAppDomain *) Object::NewPinned (il2cpp_defaults.appdomain_class);
	ad->data = domain;
	domain->domain = ad;
	domain->setup = setup;
	domain->domain_id = 1; // Only have a single domain ATM.

	domain->friendly_name = basepath(filename);

#if IL2CPP_DEBUGGER_ENABLED
	// The current thread needs to be attached before sending any debugger event.
	il2cpp_debugger_notify_appdomain_create(domain);
#endif

	LastError::InitializeLastErrorThreadStatic();

	gc::GarbageCollector::InitializeFinalizer ();

	MetadataCache::InitializeGCSafe ();
	ThreadPool::Initialize ();

	os::Socket::Startup ();

	g_il2cpp_is_fully_initialized = true;

	// Force binary serialization in Mono to use reflection instead of code generation.
	#undef SetEnvironmentVariable // Get rid of windows.h #define.
	os::Environment::SetEnvironmentVariable ("MONO_REFLECTION_SERIALIZER", "yes");
	os::Environment::SetEnvironmentVariable ("MONO_XMLSERIALIZER_THS", "no");

	Domain::ContextInit(domain);
	Domain::ContextSet(domain->default_context);
}

void Runtime::Shutdown ()
{
	ThreadPool::Shutdown ();
	
	// Foreground threads will make us wait here. Background threads
	// will get terminated abruptly.
	Thread::KillAllBackgroundThreadsAndWaitForForegroundThreads ();

	os::Socket::Cleanup ();

	os::LibraryLoader::CleanupLoadedLibraries();
	il2cpp::gc::GarbageCollector::Uninitialize ();

	// after the gc cleanup so the finalizer thread can unregister itself
	Thread::UnInitialize ();

	os::Thread::Shutdown ();

	vm::Image::ClearCachedResourceData();
	MetadataAllocCleanup ();

	os::Locale::UnInitialize();
	os::Uninitialize();
}

void Runtime::SetConfigDir (const char *path)
{
	s_ConfigDir = path;
}

void Runtime::SetDataDir(const char *path)
{
	s_DataDir = path;
}

const char *Runtime::GetFrameworkVersion ()
{
	return s_FrameworkVersion;
}

std::string Runtime::GetConfigDir ()
{
	if (s_ConfigDir.size() > 0)
		return s_ConfigDir;

	return utils::PathUtils::Combine(GetDataDir(), "etc");
}

std::string Runtime::GetDataDir()
{
	// use explicit value if set
	if (s_DataDir.size () > 0)
		return s_DataDir;

	std::string executablePath = os::Path::GetExecutablePath();
	if (!executablePath.empty())
		return utils::PathUtils::Combine(utils::PathUtils::DirectoryName(executablePath), "Data").c_str();

	if (s_DataDirFallback.size () == 0 && Environment::GetNumMainArgs () > 0)
	{
		const char* main = Environment::GetMainArgs ()[0];
		s_DataDirFallback = utils::PathUtils::DirectoryName (main);
	}

	return s_DataDirFallback;
}

Il2CppObject* Runtime::DelegateInvoke (Il2CppDelegate *delegate, void **params, Il2CppException **exc)
{
	const MethodInfo* invoke = Class::GetMethodFromName (delegate->object.klass, "Invoke", -1);
	assert (invoke);

	return Invoke (invoke, delegate, params, exc);
}

void Runtime::RaiseExecutionEngineExceptionIfMethodIsNotFound(const MethodInfo* method)
{
	if (method->methodPointer == NULL)
	{
		if (Method::GetClass(method))
			RaiseExecutionEngineException(Method::GetFullName(method).c_str());
		else
			RaiseExecutionEngineException(Method::GetName(method));
	}
}

Il2CppObject* Runtime::Invoke (const MethodInfo *method, void *obj, void **params, Il2CppException **exc)
{
	if (exc)
		*exc = NULL;

	// we wrap invoker call in try/catch here, rather than emitting a try/catch
	// in every invoke call as that blows up the code size.
	try {
		RaiseExecutionEngineExceptionIfMethodIsNotFound(method);
		return (Il2CppObject*)method->invoker_method(method, obj, params);
	}
	catch (Il2CppExceptionWrapper& ex)
	{
		if (exc)
			*exc = ex.ex;
		return NULL;
	}
}

Il2CppObject* Runtime::InvokeArray (const MethodInfo *method, void *obj, Il2CppArray *params, Il2CppException **exc)
{
	if (params == NULL)
		return InvokeConvertArgs(method, obj, NULL, 0, exc);

	// TO DO: when changing GC to one that moves managed objects around, mark params array local variable as pinned!
	return InvokeConvertArgs(method, obj, reinterpret_cast<Il2CppObject**>(Array::GetFirstElementAddress(params)), Array::GetLength(params), exc);
}

void Runtime::ObjectInit (Il2CppObject *object)
{
	ObjectInitException(object, NULL);
}

void Runtime::ObjectInitException (Il2CppObject *object, Il2CppException **exc)
{
	const MethodInfo *method = NULL;
	Il2CppClass *klass = object->klass;

	method = Class::GetMethodFromName (klass, ".ctor", 0);
	assert (method != NULL && "ObjectInit; no default constructor for object is found");

	if (method->declaring_type->valuetype)
		object = (Il2CppObject*)Object::Unbox (object);
	Invoke (method, object, NULL, exc);
}

void Runtime::SetUnhandledExceptionPolicy (Il2CppRuntimeUnhandledExceptionPolicy value)
{
	s_UnhandledExceptionPolicy = value;
}

Il2CppRuntimeUnhandledExceptionPolicy Runtime::GetUnhandledExceptionPolicy ()
{
	return s_UnhandledExceptionPolicy;
}

void Runtime::UnhandledException (Il2CppException* exc)
{
	Il2CppDomain *currentDomain = Domain::GetCurrent ();
	Il2CppDomain *rootDomain = Domain::GetRoot ();
	FieldInfo *field;
	Il2CppObject *current_appdomain_delegate = NULL;
	Il2CppObject *root_appdomain_delegate = NULL;

	field = Class::GetFieldFromName (il2cpp_defaults.appdomain_class, "UnhandledException");
	assert (field);

	Il2CppObject* excObject = (Il2CppObject*)exc;

	if (excObject->klass != il2cpp_defaults.threadabortexception_class) {
		//bool abort_process = (Thread::Current () == Thread::Main ()) ||
		//	(Runtime::GetUnhandledExceptionPolicy () == IL2CPP_UNHANDLED_POLICY_CURRENT);

		Field::GetValue ((Il2CppObject*)rootDomain->domain, field, &root_appdomain_delegate);

		NOT_IMPLEMENTED_NO_ASSERT (Runtime::UnhandledException, "We don't have runtime version info yet");
		//if (currentDomain != rootDomain && (mono_framework_version () >= 2)) {
		//	Field::GetValue ((Il2CppObject*)currentDomain->domain, field, &current_appdomain_delegate);
		//}
		//else
		//{
		//	current_appdomain_delegate = NULL;
		//}

		///* set exitcode only if we will abort the process */
		//if (abort_process)
		//	mono_environment_exitcode_set (1);
		//if ((current_appdomain_delegate == NULL) && (root_appdomain_delegate == NULL)
		//{
		//	mono_print_unhandled_exception (exc);
		//}
		//else
		{
			if (root_appdomain_delegate) {
				CallUnhandledExceptionDelegate (rootDomain, (Il2CppDelegate*)root_appdomain_delegate, exc);
			}
			if (current_appdomain_delegate) {
				CallUnhandledExceptionDelegate (currentDomain, (Il2CppDelegate*)current_appdomain_delegate, exc);
			}
		}
	}
}

static inline Il2CppObject* InvokeConvertThis (const MethodInfo* method, void* thisArg, void** convertedParameters, Il2CppException** exception)
{
	Il2CppClass* thisType = method->declaring_type;

	// If it's not a constructor, just invoke directly
	if (strcmp(method->name, ".ctor") != 0 || method->declaring_type == il2cpp_defaults.string_class)
		return Runtime::Invoke(method, thisArg, convertedParameters, exception);

	// If it is a construction, we need to construct a return value and allocate object if needed
	Il2CppObject* instance;

	if (thisArg == NULL)
	{
		thisArg = instance = Object::New(thisType);
		Runtime::Invoke(method, thisArg, convertedParameters, exception);
	}
	else
	{
		// thisArg is pointer to data in case of a value type
		// We need to invoke the constructor first, passing point to the value
		// Since the constructor may modify the value, we need to box the result
		// AFTER the constructor was invoked
		Runtime::Invoke(method, thisArg, convertedParameters, exception);
		instance = Object::Box(thisType, thisArg);
	}

	return instance;
}

Il2CppObject* Runtime::InvokeConvertArgs(const MethodInfo *method, void* thisArg, Il2CppObject** parameters, int paramCount, Il2CppException** exception)
{
	void** convertedParameters = NULL;
	bool hasByRefNullables = false;

	// Convert parameters if they are not null
	if (parameters != NULL)
	{
		convertedParameters = (void**)alloca(sizeof(void*) * paramCount);

		for (int i = 0; i < paramCount; i++)
		{
			bool passedByReference = method->parameters[i].parameter_type->byref;
			Il2CppClass* parameterType = Class::FromIl2CppType(method->parameters[i].parameter_type);
			Class::Init(parameterType);

			if (parameterType->valuetype)
			{
				if (Class::IsNullable(parameterType))
				{
					// Since we don't really store boxed nullables, we need to create a new one.
					void* nullableStorage = alloca(parameterType->instance_size - sizeof(Il2CppObject));
					Object::UnboxNullable(parameters[i], parameterType, nullableStorage);
					convertedParameters[i] = nullableStorage;
					hasByRefNullables |= passedByReference;
				}
				else if (passedByReference)
				{
					// If value type is passed by reference, just pass pointer to value directly
					// If null was passed in, create a new boxed value type in its place
					if (parameters[i] == NULL)
						parameters[i] = Object::New(parameterType);

					convertedParameters[i] = Object::Unbox(parameters[i]);
				}
				else if (parameters[i] == NULL) // If value type is passed by value, we need to pass pointer to its value
				{
					// If null was passed in, allocate a new value with default value
					uint32_t valueSize = parameterType->instance_size - sizeof(Il2CppObject);
					convertedParameters[i] = alloca(valueSize);
					memset(convertedParameters[i], 0, valueSize);
				}
				else
				{
					// Otherwise, pass the original
					convertedParameters[i] = Object::Unbox(parameters[i]);
				}
			}
			else if (passedByReference)
			{
				convertedParameters[i] = &parameters[i]; // Reference type passed by reference
			}
			else
			{
				convertedParameters[i] = parameters[i];	// Reference type passed by value
			}
		}
	}

	Il2CppObject* result = InvokeConvertThis(method, thisArg, convertedParameters, exception);

	if (hasByRefNullables)
	{
		// We need to copy by reference nullables back to original argument array
		for (int i = 0; i < paramCount; i++)
		{
			if (!method->parameters[i].parameter_type->byref)
				continue;

			Il2CppClass* parameterType = Class::FromIl2CppType(method->parameters[i].parameter_type);

			if (Class::IsNullable(parameterType))
				parameters[i] = Object::Box(parameterType, convertedParameters[i]);
		}
	}

	if (method->return_type->type == IL2CPP_TYPE_PTR)
	{
		static Il2CppClass* pointerClass = Class::FromName(il2cpp_defaults.corlib, "System.Reflection", "Pointer");
		Il2CppReflectionPointer* pointer = reinterpret_cast<Il2CppReflectionPointer*>(Object::New(pointerClass));
		pointer->data = result;
		pointer->type = Reflection::GetTypeObject(method->return_type);
		result = reinterpret_cast<Il2CppObject*>(pointer);
	}

	return result;
}

void Runtime::CallUnhandledExceptionDelegate (Il2CppDomain* domain, Il2CppDelegate* delegate, Il2CppException* exc)
{
	Il2CppException *e = NULL;
	void* pa [2];

	pa [0] = domain->domain;
	pa [1] = CreateUnhandledExceptionEventArgs (exc);
	DelegateInvoke (delegate, pa, &e);

	assert (!e);
}

static il2cpp::os::FastMutex s_TypeInitializationLock;

// We currently call Runtime::ClassInit in 4 places:
// 1. Just after we allocate storage for a new object (Object::NewAllocSpecific)
// 2. Just before reading any static field
// 3. Just before calling any static method
// 4. Just before calling class instance constructor from a derived class instance constructor
void Runtime::ClassInit (Il2CppClass *klass)
{
	// Nothing to do if class has no static constructor.
	if (!klass->has_cctor)
		return;

	// Nothing to do if class constructor already ran.
	if (os::Atomic::CompareExchange (&klass->cctor_finished, 1, 1) == 1)
		return;

	s_TypeInitializationLock.Lock ();

	// See if some thread ran it while we acquired the lock.
	if (os::Atomic::CompareExchange (&klass->cctor_finished, 1, 1) == 1)
	{
		s_TypeInitializationLock.Unlock ();
		return;
	}

	// See if some other thread got there first and already started running the constructor.
	if (os::Atomic::CompareExchange (&klass->cctor_started, 1, 1) == 1)
	{
		s_TypeInitializationLock.Unlock ();

		// May have been us and we got here through recursion.
		os::Thread::ThreadId currentThread = os::Thread::CurrentThreadId ();
		if (os::Atomic::CompareExchange64 (&klass->cctor_thread, currentThread, currentThread) == currentThread)
			return;

		// Wait for other thread to finish executing the constructor.
		while (os::Atomic::CompareExchange (&klass->cctor_finished, 1, 1) == 0)
		{
			os::Thread::Sleep (1);
		}
	}
	else
	{
		// Let others know we have started executing the constructor.
		os::Atomic::Exchange64 (&klass->cctor_thread, os::Thread::CurrentThreadId ());
		os::Atomic::Exchange (&klass->cctor_started, 1);

		s_TypeInitializationLock.Unlock ();

		// Run it.
		Il2CppException* exception = NULL;
		const MethodInfo* cctor = Class::GetCCtor (klass);
		if (cctor != NULL)
		{
			vm::Runtime::Invoke(cctor, NULL, NULL, &exception);
		}

		// Let other threads know we finished.
		os::Atomic::Exchange (&klass->cctor_finished, 1);
		os::Atomic::Exchange64 (&klass->cctor_thread, 0);

		// Deal with exceptions.
		if (exception != NULL)
		{
			const Il2CppType *type = Class::GetType (klass);
			std::string n = StringUtils::Printf ("The type initializer for '%s' threw an exception.", Type::GetName (type, IL2CPP_TYPE_NAME_FORMAT_IL).c_str());
			Il2CppException* typeInitializationException = Exception::GetTypeInitializationException (n.c_str (), exception);
			Exception::Raise (typeInitializationException);
		}
	}
}

struct ConstCharCompare
{
	bool operator()(char const *a, char const *b) const
	{
		return strcmp (a, b) < 0;
	}
};

#if IL2CPP_ENABLE_NATIVE_STACKTRACES

struct MethodInfoToMethodPointerConverter
{
	Il2CppMethodPointer operator()(const Runtime::MethodDefinitionKey& methodInfo) const
	{
		// On ARMv7 with Thumb instructions the lowest bit is always set.
		// With Thumb2 the second-to-lowest bit is also set. Mask both of
		// them off so that we can do a comparison properly based on the data
		// from the linker map file. On other architectures this operation should
		// not matter, as we assume these two bits are always zero because the pointer
		// will be aligned.
		return (Il2CppMethodPointer)((size_t)methodInfo.method & ~3);
	}
};

typedef il2cpp::utils::collections::ArrayValueMap<Il2CppMethodPointer, Runtime::MethodDefinitionKey, MethodInfoToMethodPointerConverter> NativeMethodMap;
static NativeMethodMap s_NativeMethods;

void Runtime::RegisterMethods (const std::vector<MethodDefinitionKey>& managedMethods)
{
	s_NativeMethods.assign(managedMethods);
}

#pragma pack(push, p1, 4)
struct SymbolInfo
{
	uint64_t address;
	uint32_t length;
};
#pragma pack(pop, p1)

static int32_t s_SymbolCount;
static SymbolInfo* s_SymbolInfos;
static void* s_ImageBase;

static void* LoadSymbolInfoFileFrom(const std::string& path)
{
	int error;
	os::FileHandle* handle = os::File::Open (path, os::File::kFileModeOpen, os::File::kFileAccessRead, os::File::kFileShareRead, os::File::kFileOptionsNone, &error);

	if (error != 0)
			return NULL;

	// Note that we won't unmap this file, we'll leave it open the entire lifetime of the process.
	void* mappedFile = os::MemoryMappedFile::Map(handle);

	os::File::Close(handle, &error);
	assert(error == 0);

	return mappedFile;
}


static void* LoadSymbolInfoFile ()
{
#if !IL2CPP_CAN_USE_MULTIPLE_SYMBOL_MAPS
	std::string symbolMapFileName = "SymbolMap";
#elif IL2CPP_SIZEOF_VOID_P == 4
	std::string symbolMapFileName = "SymbolMap-32";
#elif IL2CPP_SIZEOF_VOID_P == 8
	std::string symbolMapFileName = "SymbolMap-64";
#else
#error Unknown symbol map file name
#endif

	void* result = LoadSymbolInfoFileFrom(utils::PathUtils::Combine(utils::PathUtils::DirectoryName(os::Path::GetExecutablePath()), symbolMapFileName));
	if (result != NULL)
		return result;

	return LoadSymbolInfoFileFrom(utils::PathUtils::Combine(vm::Runtime::GetDataDir(), symbolMapFileName));;
}

static void InitializeSymbolInfos ()
{
	void* fileBuffer = LoadSymbolInfoFile();
	if (fileBuffer == NULL)
		return;

	s_ImageBase = os::Image::GetImageBase();
	s_SymbolCount = *((int32_t *)fileBuffer);
	s_SymbolInfos = (SymbolInfo*)((uint8_t*)fileBuffer + sizeof(s_SymbolCount));
}

static bool CompareEndOfSymbols (const SymbolInfo &a, const SymbolInfo &b)
{
	return a.address + a.length < b.address + b.length;
}

static bool s_TriedToInitializeSymbolInfo = false;

const MethodInfo* Runtime::GetMethodFromNativeSymbol (Il2CppMethodPointer nativeMethod)
{
	if (!s_TriedToInitializeSymbolInfo)
	{
		// Only attempt to initialize the symbol information once. If it is not present the first time,
		// it likely won't be there later either. Repeated chcecking can cause performance problems.
		s_TriedToInitializeSymbolInfo = true;
		InitializeSymbolInfos();
	}

	if (s_SymbolCount > 0 && s_ImageBase != NULL)
	{
		// address has to be above our base address
		if ((void*)nativeMethod < (void*)s_ImageBase)
			return NULL;

		SymbolInfo* end = s_SymbolInfos + s_SymbolCount;

		// our 'key' could be anywhere within a symbol. Our comparison function compares the end address
		// of the symbols. By doing this, upper bound returns the first symbol whose end address is greater
		// than our 'key'. This is our symbol since our end is the first end above an interior value.
		SymbolInfo interiorSymbol = { (size_t)((char*)nativeMethod - (char*)s_ImageBase), 0 };
		SymbolInfo* containingSymbol = std::upper_bound (s_SymbolInfos, end, interiorSymbol, &CompareEndOfSymbols);

		if (containingSymbol == end)
			return NULL;

		nativeMethod = (Il2CppMethodPointer)((char*)s_ImageBase + containingSymbol->address);

		// do exact lookup based on the symbol start address, as that is our key
		NativeMethodMap::iterator iter = s_NativeMethods.find_first (nativeMethod);
		if (iter != s_NativeMethods.end ())
		{
			return MetadataCache::GetMethodInfoFromMethodDefinitionIndex (iter->methodIndex);
		}
	}
	else
	{
		// get the first symbol greater than the one we want
		NativeMethodMap::iterator iter = s_NativeMethods.upper_bound (nativeMethod);

		// This will cause it to fail to pickup the last method, but we cannot do anything about it
		if (iter != s_NativeMethods.begin () && iter != s_NativeMethods.end ())
		{
			// go back one to get the symbol we actually want
			iter--;
			return MetadataCache::GetMethodInfoFromMethodDefinitionIndex(iter->methodIndex);
		}
	}

	return NULL;
}

#endif

Il2CppObject* Runtime::CreateUnhandledExceptionEventArgs (Il2CppException *exc)
{
	Il2CppClass *klass;
	void* args [2];
	const MethodInfo *method = NULL;
	bool is_terminating = true;
	Il2CppObject *obj;

	klass = Class::FromName (il2cpp_defaults.corlib, "System", "UnhandledExceptionEventArgs");
	assert (klass);

	Class::Init (klass);

	/* UnhandledExceptionEventArgs only has 1 public ctor with 2 args */
	method = Class::GetMethodFromNameFlags (klass, ".ctor", 2, METHOD_ATTRIBUTE_PUBLIC);
	assert (method);

	args [0] = exc;
	args [1] = &is_terminating;

	obj = Object::New (klass);
	Runtime::Invoke (method, obj, args, NULL);

	return obj;
}

const char *Runtime::GetBundledMachineConfig ()
{
	return s_BundledMachineConfig;
}

void Runtime::RegisterBundledMachineConfig (const char *config_xml)
{
	s_BundledMachineConfig = config_xml;
}

} /* namespace vm */
} /* namespace il2cpp */
