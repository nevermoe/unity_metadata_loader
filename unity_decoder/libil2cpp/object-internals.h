#pragma once

#include "il2cpp-config.h"
#include <stdint.h>
#include <stddef.h>

struct Il2CppClass;
struct MethodInfo;
struct PropertyInfo;
struct FieldInfo;
struct EventInfo;
struct Il2CppType;
struct Il2CppAssembly;
struct Il2CppException;
struct Il2CppImage;
struct Il2CppDomain;
struct Il2CppString;
struct Il2CppReflectionMethod;
struct Il2CppAsyncCall;
struct Il2CppIUnknown;
struct MonitorData;
struct VirtualInvokeData;

namespace il2cpp
{
	namespace os
	{
		class FastMutex;
		class Thread;
	}
}

struct Il2CppReflectionAssembly;

struct Il2CppObject
{
	Il2CppClass *klass;
	MonitorData *monitor;
};

typedef int32_t il2cpp_array_lower_bound_t;
#define IL2CPP_ARRAY_MAX_INDEX ((int32_t) 0x7fffffff)
#define IL2CPP_ARRAY_MAX_SIZE  ((uint32_t) 0xffffffff)

struct Il2CppArrayBounds
{
	il2cpp_array_size_t length;
	il2cpp_array_lower_bound_t lower_bound;
};

#if IL2CPP_COMPILER_MSVC
#pragma warning( push )
#pragma warning( disable : 4200 )
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#endif

struct Il2CppArray : public Il2CppObject
{
	/* bounds is NULL for szarrays */
	Il2CppArrayBounds *bounds;
	/* total number of elements of the array */
	il2cpp_array_size_t max_length;
};

struct Il2CppArraySize : public Il2CppArray
{
	ALIGN_TYPE(8) void* vector;
};

const size_t kIl2CppSizeOfArray = (offsetof (Il2CppArraySize, vector));
const size_t kIl2CppOffsetOfArrayBounds = (offsetof (Il2CppArray, bounds));
const size_t kIl2CppOffsetOfArrayLength = (offsetof (Il2CppArray, max_length));


// System.String
struct Il2CppString
{
	Il2CppObject object;
	int32_t length;								///< Length of string *excluding* the trailing null (which is included in 'chars').
	Il2CppChar chars [IL2CPP_ZERO_LEN_ARRAY];
};

#if IL2CPP_COMPILER_MSVC
#pragma warning( pop )
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

#define IL2CPP_OBJECT_SETREF(obj,fieldname,value) do {	\
		/* mono_gc_wbarrier_set_field ((MonoObject*)(obj), &((obj)->fieldname), (MonoObject*)value); */	\
		(obj)->fieldname = (value);	\
	} while (0)

/* This should be used if 's' can reside on the heap */
#define IL2CPP_STRUCT_SETREF(s,field,value) do { \
		/* mono_gc_wbarrier_generic_store (&((s)->field), (MonoObject*)(value)); */ \
		(s)->field = (value); \
	} while (0)

struct Il2CppReflectionType {
	Il2CppObject object;
	const Il2CppType *type;
};

// IMPORTANT: All managed types corresponding to the objects below must be blacklisted in mscorlib.xml

// System.MonoType
struct Il2CppReflectionMonoType{
	Il2CppReflectionType type;
	Il2CppObject *type_info;
};

// System.EventInfo
struct Il2CppReflectionEvent {
	Il2CppObject object;
	Il2CppObject *cached_add_event;
};

// System.Reflection.MonoEvent
struct Il2CppReflectionMonoEvent
{
	Il2CppReflectionEvent event;
	Il2CppReflectionType* reflectedType;
	const EventInfo* eventInfo;
};

// System.Reflection.MonoEventInfo
struct Il2CppReflectionMonoEventInfo
{
	Il2CppReflectionType* declaringType;
	Il2CppReflectionType* reflectedType;
	Il2CppString* name;
	Il2CppReflectionMethod* addMethod;
	Il2CppReflectionMethod* removeMethod;
	Il2CppReflectionMethod* raiseMethod;
	uint32_t eventAttributes;
	Il2CppArray* otherMethods;
};

// System.MonoEnumInfo
struct Il2CppEnumInfo {
	Il2CppReflectionType *utype;
	Il2CppArray *values;
	Il2CppArray *names;
	void* name_hash;
};

// System.Reflection.MonoField
struct Il2CppReflectionField {
	Il2CppObject object;
	Il2CppClass *klass;
	FieldInfo *field;
	Il2CppString *name;
	Il2CppReflectionType *type;
	uint32_t attrs;
};

// System.Reflection.MonoProperty
struct Il2CppReflectionProperty {
	Il2CppObject object;
	Il2CppClass *klass;
	const PropertyInfo *property;
};

// System.Reflection.MonoMethod
struct Il2CppReflectionMethod {
	Il2CppObject object;
	const MethodInfo *method;
	Il2CppString *name;
	Il2CppReflectionType *reftype;
};

// System.Reflection.MonoGenericMethod
struct Il2CppReflectionGenericMethod
{
	Il2CppReflectionMethod base;
};

// System.Reflection.MonoMethodInfo
struct Il2CppMethodInfo {
	Il2CppReflectionType *parent;
	Il2CppReflectionType *ret;
	uint32_t attrs;
	uint32_t implattrs;
	uint32_t callconv;
};

// System.Reflection.MonoPropertyInfo
struct Il2CppPropertyInfo {
	Il2CppReflectionType *parent;
	Il2CppString *name;
	Il2CppReflectionMethod *get;
	Il2CppReflectionMethod *set;
	uint32_t attrs;
};

// System.Reflection.ParameterInfo
struct Il2CppReflectionParameter{
	Il2CppObject object;
	Il2CppReflectionType *ClassImpl;
	Il2CppObject *DefaultValueImpl;
	Il2CppObject *MemberImpl;
	Il2CppString *NameImpl;
	int32_t PositionImpl;
	uint32_t AttrsImpl;
	Il2CppObject *MarshalAsImpl;
};

// System.Reflection.Module
struct Il2CppReflectionModule
{
	Il2CppObject obj;
	const Il2CppImage* image;
	Il2CppReflectionAssembly* assembly;
	Il2CppString* fqname;
	Il2CppString* name;
	Il2CppString* scopename;
	bool is_resource;
	uint32_t token;
};

// System.Reflection.AssemblyName
struct Il2CppReflectionAssemblyName
{
	Il2CppObject  obj;
	Il2CppString *name;
	Il2CppString *codebase;
	int32_t major, minor, build, revision;
	Il2CppObject  *cultureInfo;
	uint32_t     flags;
	uint32_t     hashalg;
	Il2CppObject  *keypair;
	Il2CppArray   *publicKey;
	Il2CppArray   *keyToken;
	uint32_t     versioncompat;
	Il2CppObject *version;
	uint32_t     processor_architecture;
};

// System.Reflection.Assembly
struct Il2CppReflectionAssembly {
	Il2CppObject object;
	const Il2CppAssembly *assembly;
	Il2CppObject *resolve_event_holder;
	/* CAS related */
	Il2CppObject *evidence;	/* Evidence */
	Il2CppObject *minimum;	/* PermissionSet - for SecurityAction.RequestMinimum */
	Il2CppObject *optional;	/* PermissionSet - for SecurityAction.RequestOptional */
	Il2CppObject *refuse;	/* PermissionSet - for SecurityAction.RequestRefuse */
	Il2CppObject *granted;	/* PermissionSet - for the resolved assembly granted permissions */
	Il2CppObject *denied;	/* PermissionSet - for the resolved assembly denied permissions */
	/* */
	bool from_byte_array;
	Il2CppString *name;
};

// System.Reflection.Emit.UnmanagedMarshal
struct Il2CppReflectionMarshal {
	Il2CppObject object;
	int32_t count;
	int32_t type;
	int32_t eltype;
	Il2CppString* guid;
	Il2CppString* mcookie;
	Il2CppString* marshaltype;
	Il2CppObject* marshaltyperef;
	int32_t param_num;
	bool has_size;
};

// System.Reflection.Pointer
struct Il2CppReflectionPointer
{
	Il2CppObject object;
	void* data;
	Il2CppReflectionType* type;
};

// System.IntPtr
// Just in case there is a platform where we cannot replace struct {void* val;} with void*
struct Il2CppIntPtr
{
	void* m_value;
	static Il2CppIntPtr Zero;
};

// System.Threading.Thread
struct Il2CppThread {
	Il2CppObject  obj;
	int         lock_thread_id; /* to be used as the pre-shifted thread id in thin locks */
	il2cpp::os::Thread* handle;
	Il2CppArray  *cached_culture_info;
	void*    unused1;
	bool threadpool_thread;//bool threadpool_thread;
	Il2CppChar* name;
	uint32_t name_len;
	uint32_t	    state;
	Il2CppObject* abort_exc;//MonoException *abort_exc;
	int abort_state_handle;
	uint64_t tid;	/* This is accessed as a gsize in the code (so it can hold a 64bit pointer on systems that need it), but needs to reserve 64 bits of space on all machines as it corresponds to a field in managed code */
	void*	    start_notify;
	void* stack_ptr;
	void* *static_data;
	void* jit_data;
	void* lock_data;
	Il2CppObject* current_appcontext;//MonoAppContext *current_appcontext;
	int stack_size;
	Il2CppObject* start_obj;//MonoObject *start_obj;
	void* appdomain_refs;//GSList *appdomain_refs;
	///* This is modified using atomic ops, so keep it a int32_t */
	int32_t interruption_requested;
	void* suspend_event;
	void* suspended_event;
	void* resume_event;
	il2cpp::os::FastMutex* synch_cs;
	uint8_t* serialized_culture_info;
	uint32_t serialized_culture_info_len;
	uint8_t* serialized_ui_culture_info;
	uint32_t serialized_ui_culture_info_len;
	bool thread_dump_requested;//bool thread_dump_requested;
	void* end_stack; /* This is only used when running in the debugger. */
	bool thread_interrupt_requested;//bool thread_interrupt_requested;
	uint8_t	apartment_state;
	int32_t critical_region_level;
	uint32_t small_id; /* A small, unique id, used for the hazard pointer table. */
	void* manage_callback;
	Il2CppException *pending_exception;
	Il2CppObject *ec_to_set;
	///*
	// * These fields are used to avoid having to increment corlib versions
	// * when a new field is added to the unmanaged MonoThread structure.
	// */
	void* interrupt_on_stop;
	void* unused3;
	void* unused4;
	void* unused5;
	void* unused6;
};

// System.Exception
struct Il2CppException {
	Il2CppObject object;
	/* Stores the IPs and the generic sharing infos
	   (vtable/MRGCTX) of the frames. */
	Il2CppArray *trace_ips;
	Il2CppException *inner_ex;
	Il2CppString *message;
	Il2CppString *help_link;
	Il2CppString *class_name;
	Il2CppString *stack_trace;
	Il2CppString *remote_stack_trace;
	int32_t    remote_stack_index;
	il2cpp_hresult_t hresult;
	Il2CppString *source;
	Il2CppObject *_data;
};

// System.SystemException
struct Il2CppSystemException {
	Il2CppException base;
};

// System.ArgumentException
struct Il2CppArgumentException {
	Il2CppException base;
	Il2CppString *argName;
};

// System.TypedReference
struct Il2CppTypedRef
{
	Il2CppType *type;
	void*  value;
	Il2CppClass *klass;
};

// System.Delegate
struct Il2CppDelegate {
	Il2CppObject object;
	/* The compiled code of the target method */
	Il2CppMethodPointer method_ptr;
	/* The invoke code */
	void* (*invoke_impl)(const MethodInfo*, void*, void**);
	Il2CppObject *target;
	const MethodInfo *method;
	void* delegate_trampoline;
	/*
	 * If non-NULL, this points to a memory location which stores the address of
	 * the compiled code of the method, or NULL if it is not yet compiled.
	 */
	uint8_t **method_code;
	Il2CppReflectionMethod *method_info;
	Il2CppReflectionMethod *original_method_info;
	Il2CppObject *data;
};

// System.MarshalByRefObject
struct Il2CppMarshalByRefObject {
	Il2CppObject obj;
	Il2CppObject *identity;
};

// System.__Il2CppComObject (dummy type that replaces System.__ComObject)
struct Il2CppComObject : Il2CppObject
{
	Il2CppIUnknown* identity;
};

// System.AppDomain
struct Il2CppAppDomain {
	Il2CppMarshalByRefObject mbr;
	Il2CppDomain *data;
};

// System.Diagnostics.StackFrame
struct Il2CppStackFrame {
	Il2CppObject obj;
	int32_t il_offset;
	int32_t native_offset;
	Il2CppReflectionMethod *method;
	Il2CppString *filename;
	int32_t line;
	int32_t column;
	Il2CppString *internal_method_name;
};

// System.Globalization.DateTimeFormatInfo
struct Il2CppDateTimeFormatInfo {
	Il2CppObject obj;
	bool readOnly;
	Il2CppString* AMDesignator;
	Il2CppString* PMDesignator;
	Il2CppString* DateSeparator;
	Il2CppString* TimeSeparator;
	Il2CppString* ShortDatePattern;
	Il2CppString* LongDatePattern;
	Il2CppString* ShortTimePattern;
	Il2CppString* LongTimePattern;
	Il2CppString* MonthDayPattern;
	Il2CppString* YearMonthPattern;
	Il2CppString* FullDateTimePattern;
	Il2CppString* RFC1123Pattern;
	Il2CppString* SortableDateTimePattern;
	Il2CppString* UniversalSortableDateTimePattern;
	uint32_t FirstDayOfWeek;
	Il2CppObject* Calendar;
	uint32_t CalendarWeekRule;
	Il2CppArray* AbbreviatedDayNames;
	Il2CppArray* DayNames;
	Il2CppArray* MonthNames;
	Il2CppArray* AbbreviatedMonthNames;
	Il2CppArray* ShortDatePatterns;
	Il2CppArray* LongDatePatterns;
	Il2CppArray* ShortTimePatterns;
	Il2CppArray* LongTimePatterns;
	Il2CppArray* MonthDayPatterns;
	Il2CppArray* YearMonthPatterns;
	Il2CppArray* shortDayNames;
};

// System.Globalization.NumberFormatInfo
struct Il2CppNumberFormatInfo {
	Il2CppObject obj;
	bool readOnly;
	Il2CppString* decimalFormats;
	Il2CppString* currencyFormats;
	Il2CppString* percentFormats;
	Il2CppString* digitPattern;
	Il2CppString* zeroPattern;
	int32_t currencyDecimalDigits;
	Il2CppString* currencyDecimalSeparator;
	Il2CppString* currencyGroupSeparator;
	Il2CppArray* currencyGroupSizes;
	int32_t currencyNegativePattern;
	int32_t currencyPositivePattern;
	Il2CppString* currencySymbol;
	Il2CppString* naNSymbol;
	Il2CppString* negativeInfinitySymbol;
	Il2CppString* negativeSign;
	uint32_t numberDecimalDigits;
	Il2CppString* numberDecimalSeparator;
	Il2CppString* numberGroupSeparator;
	Il2CppArray* numberGroupSizes;
	int32_t numberNegativePattern;
	int32_t percentDecimalDigits;
	Il2CppString* percentDecimalSeparator;
	Il2CppString* percentGroupSeparator;
	Il2CppArray* percentGroupSizes;
	int32_t percentNegativePattern;
	int32_t percentPositivePattern;
	Il2CppString* percentSymbol;
	Il2CppString* perMilleSymbol;
	Il2CppString* positiveInfinitySymbol;
	Il2CppString* positiveSign;
};

// System.Globalization.CultureInfo
struct Il2CppCultureInfo {
	Il2CppObject obj;
	bool is_read_only;
	int32_t lcid;
	int32_t parent_lcid;
	int32_t specific_lcid;
	int32_t datetime_index;
	int32_t number_index;
	bool use_user_override;
	Il2CppNumberFormatInfo* number_format;
	Il2CppDateTimeFormatInfo* datetime_format;
	Il2CppObject* textinfo;
	Il2CppString* name;
	Il2CppString* displayname;
	Il2CppString* englishname;
	Il2CppString* nativename;
	Il2CppString* iso3lang;
	Il2CppString* iso2lang;
	Il2CppString* icu_name;
	Il2CppString* win3lang;
	Il2CppString* territory;
	Il2CppString* compareinfo;
	const int32_t* calendar_data;
	const void* text_info_data;
};

// System.Threading.WaitHandle
struct Il2CppWaitHandle {
	Il2CppMarshalByRefObject object;
	void* handle;
	bool disposed;
};

// System.Runtime.InteropServices.SafeHandle
// Inherited by Microsoft.Win32.SafeHandles.SafeWaitHandle
struct Il2CppSafeHandle {
	Il2CppObject base;
	void* handle;
	void* invalid_handle_value;
	int refcount;
	bool owns_handle;
};

// System.Text.StringBuilder
struct Il2CppStringBuilder {
	Il2CppObject object;
	int32_t length;
	Il2CppString *str;
	Il2CppString *cached_str;
	int32_t max_capacity;
};

// System.Net.SocketAddress
struct Il2CppSocketAddress
{
	Il2CppObject base;
	Il2CppArray* data;
};

// System.Globalization.SortKey
struct Il2CppSortKey
{
	Il2CppObject base;
	Il2CppString *str;
	int32_t options;
	Il2CppArray *key;
	int32_t lcid;
};

// System.Runtime.InteropServices.ErrorWrapper
struct Il2CppErrorWrapper
{
	Il2CppObject base;
	int32_t errorCode;
};

struct Il2CppAsyncResult
{
	Il2CppObject base;
	Il2CppObject *async_state;
	Il2CppWaitHandle *handle;
	Il2CppDelegate *async_delegate;
	void* data; // We pass delegate arguments here. This is repurposed. Depends on Mono C# code not using the field.
	Il2CppAsyncCall *object_data;
	bool  sync_completed;
	bool  completed;
	bool  endinvoke_called;
	Il2CppObject *async_callback;
	Il2CppObject *execution_context;
	Il2CppObject *original_context;
};

struct Il2CppAsyncCall
{
	Il2CppObject base;
	void *msg; // We pass exceptions through here for now.
	MethodInfo *cb_method; // We don't set this.
	Il2CppDelegate *cb_target; // We pass the actual delegate here.
	Il2CppObject *state;
	Il2CppObject *res;
	Il2CppArray *out_args;
	/* This is a HANDLE, we use guint64 so the managed object layout remains constant */
	uint64_t wait_event;
};

struct Il2CppExceptionWrapper
{
	Il2CppException* ex;

	Il2CppExceptionWrapper (Il2CppException* ex) : ex (ex) {}
};

/// Corresponds to Mono's internal System.Net.Sockets.Socket.SocketAsyncResult
/// class. Has no relation to Il2CppAsyncResult.
struct Il2CppSocketAsyncResult
{
	Il2CppObject base;
	Il2CppObject *socket;
	Il2CppIntPtr handle;
	Il2CppObject *state;
	Il2CppDelegate *callback;
	Il2CppWaitHandle *wait_handle;
	Il2CppException *delayed_exc;
	Il2CppObject *ep;
	Il2CppArray *buffer;
	int32_t offset;
	int32_t size;
	int32_t socket_flags;
	Il2CppObject *accept_reuse_socket;
	Il2CppArray *addresses;
	int32_t port;
	Il2CppObject *buffers;
	bool reusesocket;
	Il2CppObject *acc_socket;
	int32_t total;
	bool completed_synch;
	bool completed;
	bool blocking;
	int32_t error;
	int32_t operation;
	Il2CppAsyncResult *ares;
};

enum Il2CppResourceLocation
{
	RESOURCE_LOCATION_EMBEDDED = 1,
	RESOURCE_LOCATION_ANOTHER_ASSEMBLY = 2,
	RESOURCE_LOCATION_IN_MANIFEST = 4
};

struct Il2CppManifestResourceInfo
{
	Il2CppObject object;
	Il2CppReflectionAssembly* assembly;
	Il2CppString* filename;
	uint32_t location;
};

#define IL2CPP_CHECK_ARG_NULL(arg)	do {	\
	if (arg == NULL)	\
	{	\
		il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetArgumentNullException (#arg));	\
	};	} while (0)

struct Il2CppAppContext 
{
	Il2CppObject obj;
	int32_t domain_id;
	int32_t context_id;
	void* static_data;
};

struct Il2CppGuid
{
	uint32_t data1;
	uint16_t data2;
	uint16_t data3;
	uint8_t data4[8];
};

struct Il2CppSafeArrayBound
{
	uint32_t element_count;
	int32_t lower_bound;
};

struct Il2CppSafeArray
{
	uint16_t dimention_count;
	uint16_t features;
	uint32_t element_size;
	uint32_t lock_count;
	void* data;
	Il2CppSafeArrayBound bounds[1];
};

struct Il2CppDecimal
{
	uint16_t reserved;
	union
	{
		struct
		{
			uint8_t scale;
			uint8_t sign;
		} s;
		uint16_t signscale;
	} u;
	uint32_t hi32;
	union
	{
		struct
		{
			uint32_t lo32;
			uint32_t mid32;
		} s2;
		uint64_t lo64;
	} u2;
};

typedef int16_t IL2CPP_VARIANT_BOOL;

#define IL2CPP_VARIANT_TRUE ((IL2CPP_VARIANT_BOOL)-1)
#define IL2CPP_VARIANT_FALSE ((IL2CPP_VARIANT_BOOL)0)

enum Il2CppVarType
{
	IL2CPP_VT_EMPTY = 0,
	IL2CPP_VT_NULL = 1,
	IL2CPP_VT_I2 = 2,
	IL2CPP_VT_I4 = 3,
	IL2CPP_VT_R4 = 4,
	IL2CPP_VT_R8 = 5,
	IL2CPP_VT_CY = 6,
	IL2CPP_VT_DATE = 7,
	IL2CPP_VT_BSTR = 8,
	IL2CPP_VT_DISPATCH = 9,
	IL2CPP_VT_ERROR = 10,
	IL2CPP_VT_BOOL = 11,
	IL2CPP_VT_VARIANT = 12,
	IL2CPP_VT_UNKNOWN = 13,
	IL2CPP_VT_DECIMAL = 14,
	IL2CPP_VT_I1 = 16,
	IL2CPP_VT_UI1 = 17,
	IL2CPP_VT_UI2 = 18,
	IL2CPP_VT_UI4 = 19,
	IL2CPP_VT_I8 = 20,
	IL2CPP_VT_UI8 = 21,
	IL2CPP_VT_INT = 22,
	IL2CPP_VT_UINT = 23,
	IL2CPP_VT_VOID = 24,
	IL2CPP_VT_HRESULT = 25,
	IL2CPP_VT_PTR = 26,
	IL2CPP_VT_SAFEARRAY = 27,
	IL2CPP_VT_CARRAY = 28,
	IL2CPP_VT_USERDEFINED = 29,
	IL2CPP_VT_LPSTR = 30,
	IL2CPP_VT_LPWSTR = 31,
	IL2CPP_VT_RECORD = 36,
	IL2CPP_VT_INT_PTR = 37,
	IL2CPP_VT_UINT_PTR = 38,
	IL2CPP_VT_FILETIME = 64,
	IL2CPP_VT_BLOB = 65,
	IL2CPP_VT_STREAM = 66,
	IL2CPP_VT_STORAGE = 67,
	IL2CPP_VT_STREAMED_OBJECT = 68,
	IL2CPP_VT_STORED_OBJECT = 69,
	IL2CPP_VT_BLOB_OBJECT = 70,
	IL2CPP_VT_CF = 71,
	IL2CPP_VT_CLSID = 72,
	IL2CPP_VT_VERSIONED_STREAM = 73,
	IL2CPP_VT_BSTR_BLOB = 0xfff,
	IL2CPP_VT_VECTOR = 0x1000,
	IL2CPP_VT_ARRAY = 0x2000,
	IL2CPP_VT_BYREF = 0x4000,
	IL2CPP_VT_RESERVED = 0x8000,
	IL2CPP_VT_ILLEGAL = 0xffff,
	IL2CPP_VT_ILLEGALMASKED = 0xfff,
	IL2CPP_VT_TYPEMASK = 0xfff,
};

struct Il2CppVariant
{
	union
	{
		struct __tagVARIANT
		{
			uint16_t type;
			uint16_t reserved1;
			uint16_t reserved2;
			uint16_t reserved3;
			union
			{
				int64_t llVal;
				int32_t lVal;
				uint8_t bVal;
				int16_t iVal;
				float fltVal;
				double dblVal;
				IL2CPP_VARIANT_BOOL boolVal;
				int32_t scode;
				int64_t cyVal;
				double date;
				Il2CppChar* bstrVal;
				Il2CppIUnknown* punkVal;
				void* pdispVal;
				Il2CppSafeArray* parray;
				uint8_t* pbVal;
				int16_t* piVal;
				int32_t* plVal;
				int64_t* pllVal;
				float* pfltVal;
				double* pdblVal;
				IL2CPP_VARIANT_BOOL* pboolVal;
				int32_t* pscode;
				int64_t* pcyVal;
				double* pdate;
				Il2CppChar* pbstrVal;
				Il2CppIUnknown** ppunkVal;
				void** ppdispVal;
				Il2CppSafeArray** pparray;
				Il2CppVariant* pvarVal;
				void* byref;
				char cVal;
				uint16_t uiVal;
				uint32_t ulVal;
				uint64_t ullVal;
				int intVal;
				unsigned int uintVal;
				Il2CppDecimal* pdecVal;
				char* pcVal;
				uint16_t* puiVal;
				uint32_t* pulVal;
				uint64_t* pullVal;
				int* pintVal;
				unsigned int* puintVal;
				struct __tagBRECORD
				{
					void* pvRecord;
					void* pRecInfo;
				} n4;
			} n3;
		} n2;
		Il2CppDecimal decVal;
	} n1;
};

struct Il2CppFileTime
{
	uint32_t low;
	uint32_t high;
};

struct Il2CppStatStg
{
	Il2CppChar* name;
	uint32_t type;
	uint64_t size;
	Il2CppFileTime mtime;
	Il2CppFileTime ctime;
	Il2CppFileTime atime;
	uint32_t mode;
	uint32_t locks;
	Il2CppGuid clsid;
	uint32_t state;
	uint32_t reserved;
};

struct Il2CppHString__
{
	int unused;
};

typedef Il2CppHString__* Il2CppHString;

struct Il2CppHStringHeader
{
	union
	{
		void* Reserved1;
#if IL2CPP_SIZEOF_VOID_P == 8
		char Reserved2[24];
#else
		char Reserved2[20];
#endif
	} Reserved;
};


struct NOVTABLE Il2CppIUnknown
{
	static const LIBIL2CPP_CODEGEN_API Il2CppGuid IID;
	virtual il2cpp_hresult_t STDCALL QueryInterface(const Il2CppGuid& iid, void** object) = 0;
	virtual uint32_t STDCALL AddRef() = 0;
	virtual uint32_t STDCALL Release() = 0;
};

struct NOVTABLE Il2CppISequentialStream : Il2CppIUnknown
{
	static const LIBIL2CPP_CODEGEN_API Il2CppGuid IID;
	virtual il2cpp_hresult_t STDCALL Read(void* buffer, uint32_t size, uint32_t* read) = 0;
	virtual il2cpp_hresult_t STDCALL Write(const void* buffer, uint32_t size, uint32_t* written) = 0;
};

struct NOVTABLE Il2CppIStream : Il2CppISequentialStream
{
	static const LIBIL2CPP_CODEGEN_API Il2CppGuid IID;
	virtual il2cpp_hresult_t STDCALL Seek(int64_t move, uint32_t origin, uint64_t* position) = 0;
	virtual il2cpp_hresult_t STDCALL SetSize(uint64_t size) = 0;
	virtual il2cpp_hresult_t STDCALL CopyTo(Il2CppIStream* stream, uint64_t size, uint64_t* read, uint64_t* written) = 0;
	virtual il2cpp_hresult_t STDCALL Commit(uint32_t flags) = 0;
	virtual il2cpp_hresult_t STDCALL Revert() = 0;
	virtual il2cpp_hresult_t STDCALL LockRegion(uint64_t offset, uint64_t size, uint32_t type) = 0;
	virtual il2cpp_hresult_t STDCALL UnlockRegion(uint64_t offset, uint64_t size, uint32_t type) = 0;
	virtual il2cpp_hresult_t STDCALL Stat(Il2CppStatStg* data, uint32_t flags) = 0;
	virtual il2cpp_hresult_t STDCALL Clone(Il2CppIStream** stream) = 0;
};

struct NOVTABLE Il2CppIMarshal : Il2CppIUnknown
{
	static const LIBIL2CPP_CODEGEN_API Il2CppGuid IID;
	virtual il2cpp_hresult_t STDCALL GetUnmarshalClass(const Il2CppGuid& iid, void* object, uint32_t context, void* reserved, uint32_t flags, Il2CppGuid* clsid) = 0;
	virtual il2cpp_hresult_t STDCALL GetMarshalSizeMax(const Il2CppGuid& iid, void* object, uint32_t context, void* reserved, uint32_t flags, uint32_t* size) = 0;
	virtual il2cpp_hresult_t STDCALL MarshalInterface(Il2CppIStream* stream, const Il2CppGuid& iid, void* object, uint32_t context, void* reserved, uint32_t flags) = 0;
	virtual il2cpp_hresult_t STDCALL UnmarshalInterface(Il2CppIStream* stream, const Il2CppGuid& iid, void** object) = 0;
	virtual il2cpp_hresult_t STDCALL ReleaseMarshalData(Il2CppIStream* stream) = 0;
	virtual il2cpp_hresult_t STDCALL DisconnectObject(uint32_t reserved) = 0;
};

struct NOVTABLE Il2CppIManagedObject : Il2CppIUnknown
{
	static const LIBIL2CPP_CODEGEN_API Il2CppGuid IID;
	virtual il2cpp_hresult_t STDCALL GetSerializedBuffer(Il2CppChar** bstr) = 0;
	virtual il2cpp_hresult_t STDCALL GetObjectIdentity(Il2CppChar** bstr_guid, int32_t* app_domain_id, intptr_t* ccw) = 0;
};

struct NOVTABLE Il2CppIInspectable : Il2CppIUnknown
{
	static const LIBIL2CPP_CODEGEN_API Il2CppGuid IID;
	virtual il2cpp_hresult_t STDCALL GetIids(uint32_t* iidCount, Il2CppGuid** iids) = 0;
	virtual il2cpp_hresult_t STDCALL GetRuntimeClassName(Il2CppHString* className) = 0;
	virtual il2cpp_hresult_t STDCALL GetTrustLevel(int32_t* trustLevel) = 0;
};

struct NOVTABLE Il2CppIActivationFactory : Il2CppIInspectable
{
	static const LIBIL2CPP_CODEGEN_API Il2CppGuid IID;
	virtual il2cpp_hresult_t STDCALL ActivateInstance(Il2CppIInspectable** instance) = 0;
};