#pragma once

#include <set>
#include <list>

#include "common.h"
#include "thread-data.h"
#include "protocol/protocol.h"
#include "event-request.h"
#include "il2cpp-api-test-forwarder.h"

#include "../os/Thread.h"
#include "../os/Semaphore.h"

struct Il2CppStackFrameInfo;

namespace il2cpp
{
namespace debugger
{

class Event;
class Reply;
class Command;
class IDataTransport;

struct StackFrameGetValuesCommand;
struct StackFrameSetValuesCommand;
struct StackFrameGetThisCommand;
struct AssemblyGetObjectCommand;
struct AssemblyGetEntryPointCommand;
struct AssemblyGetManifestModuleCommand;
struct AssemblyGetNameCommand;
struct AssemblyGetLocationCommand;
struct AssemblyGetTypeCommand;
struct InternalErrorNotImplementedCommand;
struct InternalErrorInvalidCommand;
struct VmSetProtocolVersionCommand;
struct VmSuspendCommand;
struct VmAllThreadsCommand;
struct VmVersionCommand;
struct VmDisposeCommand;
struct VmAbortInvokeCommand;
struct VmResumeCommand;
struct VmExitCommand;
struct VmInvokeMethodCommand;
struct StringRefGetValueCommand;
struct ObjectRefGetTypeCommand;
struct ObjectRefGetDomainCommand;
struct ObjectRefIsCollectedCommand;
struct ObjectRefSetValuesCommand;
struct ObjectRefGetAddressCommand;
struct ObjectRefGetValuesCommand;
struct TypeGetPropertiesCommand;
struct TypeGetSourceFiles2Command;
struct TypeGetPropertyCAttrsCommand;
struct TypeGetFieldCAttrsCommand;
struct TypeGetInfoCommand;
struct TypeIsAssignableFromCommand;
struct TypeGetCAttrsCommand;
struct TypeGetSourceFilesCommand;
struct TypeGetValuesCommand;
struct TypeGetObjectCommand;
struct TypeGetFieldsCommand;
struct TypeSetValuesCommand;
struct TypeGetMethodsCommand;
struct ThreadGetNameCommand;
struct ThreadGetFrameInfoCommand;
struct ThreadGetInfoCommand;
struct ThreadGetStateCommand;
struct ThreadGetIdCommand;
struct ArrayRefGetValuesCommand;
struct ArrayRefGetLengthCommand;
struct ArrayRefSetValuesCommand;
struct EventRequestClearCommand;
struct EventRequestClearAllBreakpointsCommand;
struct EventRequestSetCommand;
struct AppDomainGetFriendlyNameCommand;
struct AppDomainCreateStringCommand;
struct AppDomainGetEntryAssemblyCommand;
struct AppDomainGetRootDomainCommand;
struct AppDomainCreateBoxedValueCommand;
struct AppDomainGetAssembliesCommand;
struct AppDomainGetCorlibCommand;
struct MethodGetParamInfoCommand;
struct MethodGetNameCommand;
struct MethodGetDeclaringTypeCommand;
struct MethodGetLocalsInfoCommand;
struct MethodGetInfoCommand;
struct MethodGetDebugInfoCommand;
struct MethodResolveTokenCommand;
struct MethodGetBodyCommand;
struct ModuleGetInfoCommand;
class EventCompositeCommand;

struct AgentConfig
{
	bool server;
	const char *host;
	int32_t port;

	AgentConfig();
	AgentConfig(const char *host, int32_t port);

	static AgentConfig Default;
};

static int32_t suspend_count = 0;
static os::Semaphore suspend_semaphore = 0;

#define INVALIDATE_BREAKPOINT_DATA(bp)	\
	(bp)->method = (MethodInfo*)0xCAFFECAFFE

#define IS_BREAKPOINT_DATA_INVALID(bp)	\
	((bp) != 0 && (bp)->method != (MethodInfo*)0xCAFFECAFFE)

struct BreakpointData
{
	const MethodInfo *method;
	const EventRequest *request;
	uint64_t location;
};

struct SingleStepData
{
	StepSizeKind size;
	StepDepthKind depth;
	const Il2CppThread *thread;
	const EventRequest *request;
	const MethodInfo *last_method;
	int32_t last_line;
	int32_t stack_depth;
};

class Agent
{

public:

	inline static Agent &instance()
	{
		return Agent::_instance;
	}

	void Initialize(const AgentConfig &config);
	void Dispose();
	bool IsInitialized();

	void ListenForClientMessages();
	void SequencePointHit(int64_t uid, int32_t offset);
	void NotifyVmStart();
	void NotifyVmDeath();
	void NotifyUserBreak();
	void NotifyBreakPointHit(const MethodInfo *method, uint64_t offset, BreakpointData &bp_data);
	void NotifyStepHit(const MethodInfo *method, uint64_t offset, SingleStepData &ss_data);
	void NotifyAppDomainCreate(Il2CppDomain *domain);
	void NotifyAssemblyLoad(const Il2CppAssembly *assembly);
	void NotifyTypeLoad(Il2CppClass *type);
	void NotifyThreadStart(Il2CppThread *thread);
	void NotifyThreadDeath(Il2CppThread *thread);

	void OnMethodEntry(const Il2CppStackFrameInfo &info);
	void OnMethodExit(const Il2CppStackFrameInfo &info);

	void OnThreadAttach(Il2CppThread *thread);
	void OnThreadDetach(Il2CppThread *thread);

	bool IsDebuggerThread();

	const Reply *Process(const Command *command);
	const Protocol& GetProtocol();

	bool IsSuspended();
	void RequestSuspend();
	void RequestResume();
	void SuspendCurrentThreadIfNeeded();

#if IL2CPP_UNIT_TESTS_ENABLED
	static Il2CppApiTestForwarder TestForwarder;
#endif // IL2CPP_UNIT_TESTS_ENABLED

private:

	static Agent _instance;

	Agent();
	~Agent();

	void InvalidateBreakpointData();
	void ClearBreakpointData();
	void ClearSingleStepData();

	void StartListeningThread();
	void StopListeningThread();

	void WaitForSuspend();
	bool ProcessSingleStepIfNeeded(int64_t uid, int32_t offset);
	bool ShouldProcessStepRequest(Il2CppThread *current_thread, const Il2CppStackFrameInfo *top_frame, int32_t stack_depth, bool *can_break_at_the_same_line);
	BreakpointData *BreakpointDataAt(int64_t uid, int32_t offset);

	void NotifyAllPendingEvents();
	void NotifyAppDomainCreatePendingEvents();
	void NotifyAssemblyLoadPendingEvents();
	void NotifyTypeLoadPendingEvents();
	void NotifyThreadStartPendingEvents();
	void NotifyPendingEventsOfKind(EventKind kind);

	void NotifyEvent(const Event &evt, EventRequestList &event_reqs);
	void OnEventSkipped(const Event &evt);

	void InvokeMethodIfOneIsReady(ThreadData* current_thread_data);

	SingleStepData *PrepareSingleStepData(EventRequest *request, Il2CppThread *thread, StepSizeKind size, StepDepthKind depth);

	bool ShouldSendPendingEventsOnFirstMethodEntry();
	
	const Reply *Process(const StackFrameGetValuesCommand *command);
	const Reply *Process(const StackFrameSetValuesCommand *command);
	const Reply *Process(const StackFrameGetThisCommand *command);
	const Reply *Process(const AssemblyGetObjectCommand *command);
	const Reply *Process(const AssemblyGetEntryPointCommand *command);
	const Reply *Process(const AssemblyGetManifestModuleCommand *command);
	const Reply *Process(const AssemblyGetNameCommand *command);
	const Reply *Process(const AssemblyGetLocationCommand *command);
	const Reply *Process(const AssemblyGetTypeCommand *command);
	const Reply *Process(const InternalErrorNotImplementedCommand *command);
	const Reply *Process(const VmSetProtocolVersionCommand *command);
	const Reply *Process(const VmSuspendCommand *command);
	const Reply *Process(const VmAllThreadsCommand *command);
	const Reply *Process(const VmVersionCommand *command);
	const Reply *Process(const VmDisposeCommand *command);
	const Reply *Process(const VmAbortInvokeCommand *command);
	const Reply *Process(const VmResumeCommand *command);
	const Reply *Process(const VmExitCommand *command);
	const Reply *Process(const VmInvokeMethodCommand *command);
	const Reply *Process(const StringRefGetValueCommand *command);
	const Reply *Process(const ObjectRefGetTypeCommand *command);
	const Reply *Process(const ObjectRefGetDomainCommand *command);
	const Reply *Process(const ObjectRefIsCollectedCommand *command);
	const Reply *Process(const ObjectRefSetValuesCommand *command);
	const Reply *Process(const ObjectRefGetAddressCommand *command);
	const Reply *Process(const ObjectRefGetValuesCommand *command);
	const Reply *Process(const TypeGetPropertiesCommand *command);
	const Reply *Process(const TypeGetSourceFiles2Command *command);
	const Reply *Process(const TypeGetPropertyCAttrsCommand *command);
	const Reply *Process(const TypeGetFieldCAttrsCommand *command);
	const Reply *Process(const TypeGetInfoCommand *command);
	const Reply *Process(const TypeIsAssignableFromCommand *command);
	const Reply *Process(const TypeGetCAttrsCommand *command);
	const Reply *Process(const TypeGetSourceFilesCommand *command);
	const Reply *Process(const TypeGetValuesCommand *command);
	const Reply *Process(const TypeGetObjectCommand *command);
	const Reply *Process(const TypeGetFieldsCommand *command);
	const Reply *Process(const TypeSetValuesCommand *command);
	const Reply *Process(const TypeGetMethodsCommand *command);
	const Reply *Process(const ThreadGetNameCommand *command);
	const Reply *Process(const ThreadGetFrameInfoCommand *command);
	const Reply *Process(const ThreadGetInfoCommand *command);
	const Reply *Process(const ThreadGetStateCommand *command);
	const Reply *Process(const ThreadGetIdCommand *command);
	const Reply *Process(const ArrayRefGetValuesCommand *command);
	const Reply *Process(const ArrayRefGetLengthCommand *command);
	const Reply *Process(const ArrayRefSetValuesCommand *command);
	const Reply *Process(const EventRequestClearCommand *command);
	const Reply *Process(const EventRequestClearAllBreakpointsCommand *command);
	const Reply *Process(const EventRequestSetCommand *command);
	const Reply *Process(const AppDomainGetFriendlyNameCommand *command);
	const Reply *Process(const AppDomainCreateStringCommand *command);
	const Reply *Process(const AppDomainGetEntryAssemblyCommand *command);
	const Reply *Process(const AppDomainGetRootDomainCommand *command);
	const Reply *Process(const AppDomainCreateBoxedValueCommand *command);
	const Reply *Process(const AppDomainGetAssembliesCommand *command);
	const Reply *Process(const AppDomainGetCorlibCommand *command);
	const Reply *Process(const MethodGetParamInfoCommand *command);
	const Reply *Process(const MethodGetNameCommand *command);
	const Reply *Process(const MethodGetDeclaringTypeCommand *command);
	const Reply *Process(const MethodGetLocalsInfoCommand *command);
	const Reply *Process(const MethodGetInfoCommand *command);
	const Reply *Process(const MethodGetDebugInfoCommand *command);
	const Reply *Process(const MethodResolveTokenCommand *command);
	const Reply *Process(const MethodGetBodyCommand *command);
	const Reply *Process(const ModuleGetInfoCommand *command);
	const Reply *Process(const EventCompositeCommand *command);

	AgentConfig _config;
	Protocol _protocol;
	os::Thread *_thread;
	Il2CppThread *_managed_debugger_thread;
	EventRequestList _event_req_list;
	IDataTransport *_transport;
	bool _is_notifying_pending_events;
	std::list<BreakpointData*> _bp_data;
	SingleStepData *_ss_data;
	bool _notify_pending_events_on_first_invoke;
	bool _is_initialized;
	bool _currently_invoking_method;

	std::set<Il2CppClass*> _pending_types;
	std::set<Il2CppThread*> _pending_threads;
	std::set<Il2CppDomain*> _pending_domains;
	std::set<Il2CppAssembly*> _pending_assemblies;

	ThreadDataMap _thread_data;

	DISALLOW_COPY(Agent);
};

class CurrentlyInvokingMethodHelper
{
public:
	CurrentlyInvokingMethodHelper(bool& currently_invoking_method) :
		_currently_invoking_method(currently_invoking_method)
	{
		_currently_invoking_method = true;
	}

	~CurrentlyInvokingMethodHelper()
	{
		_currently_invoking_method = false;
	}

private:
	bool& _currently_invoking_method;
};

} /* namespace debugger */
} /* namespace il2cpp */

