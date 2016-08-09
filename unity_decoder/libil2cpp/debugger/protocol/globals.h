#pragma once

#include "../common.h"

namespace il2cpp
{
namespace debugger
{

enum CommandSet {
	kCommandSetInternalError = -1,
	kCommandSetVm = 1,
	kCommandSetObjectRef = 9,
	kCommandSetStringRef = 10,
	kCommandSetThread = 11,
	kCommandSetArrayRef = 13,
	kCommandSetEventRequest = 15,
	kCommandSetStackFrame = 16,
	kCommandSetAppDomain = 20,
	kCommandSetAssembly = 21,
	kCommandSetMethod = 22,
	kCommandSetType = 23,
	kCommandSetModule = 24,
	kCommandSetEvent = 64
};

enum CommandSetInternalError {
	kCommandSetInternalErrorNotImplemented = 1
};

enum CommandSetVm {
	kCommandSetVmVersion = 1,
	kCommandSetVmAllThreads = 2,
	kCommandSetVmSuspend = 3,
	kCommandSetVmResume = 4,
	kCommandSetVmExit = 5,
	kCommandSetVmDispose = 6,
	kCommandSetVmInvokeMethod = 7,
	kCommandSetVmSetProtocolVersion = 8,
	kCommandSetVmAbortInvoke = 9
};

enum CommandSetThread {
	kCommandSetThreadGetFrameInfo = 1,
	kCommandSetThreadGetName = 2,
	kCommandSetThreadGetState = 3,
	kCommandSetThreadGetInfo = 4,
	kCommandSetThreadGetId = 5
};

enum CommandSetEventRequest {
	kCommandSetEventRequestSet = 1,
	kCommandSetEventRequestClear = 2,
	kCommandSetEventRequestClearAllBreakpoints = 3
};

enum CommandSetEvent {
	kCommandSetEventComposite = 100
};

enum CommandSetAppDomain {
	kCommandSetAppDomainGetRootDomain = 1,
	kCommandSetAppDomainGetFriendlyName = 2,
	kCommandSetAppDomainGetAssemblies = 3,
	kCommandSetAppDomainGetEntryAssembly = 4,
	kCommandSetAppDomainCreateString = 5,
	kCommandSetAppDomainGetCorlib = 6,
	kCommandSetAppDomainCreateBoxedValue = 7,
};

enum CommandSetAssembly {
	kCommandSetAssemblyGetLocation = 1,
	kCommandSetAssemblyGetEntryPoint = 2,
	kCommandSetAssemblyGetManifestModule = 3,
	kCommandSetAssemblyGetObject = 4,
	kCommandSetAssemblyGetType = 5,
	kCommandSetAssemblyGetName = 6
};

enum CommandSetModule {
	kCommandSetModuleGetInfo = 1,
};

enum CommandSetMethod {
	kCommandSetMethodGetName = 1,
	kCommandSetMethodGetDeclaringType = 2,
	kCommandSetMethodGetDebugInfo = 3,
	kCommandSetMethodGetParamInfo = 4,
	kCommandSetMethodGetLocalsInfo = 5,
	kCommandSetMethodGetInfo = 6,
	kCommandSetMethodGetBody = 7,
	kCommandSetMethodResolveToken = 8,
};

enum CommandSetType {
	kCommandSetTypeGetInfo = 1,
	kCommandSetTypeGetMethods = 2,
	kCommandSetTypeGetFields = 3,
	kCommandSetTypeGetValues = 4,
	kCommandSetTypeGetObject = 5,
	kCommandSetTypeGetSourceFiles = 6,
	kCommandSetTypeSetValues = 7,
	kCommandSetTypeIsAssignableFrom = 8,
	kCommandSetTypeGetProperties = 9,
	kCommandSetTypeGetCAttrs = 10,
	kCommandSetTypeGetFieldCAttrs = 11,
	kCommandSetTypeGetPropertyCAttrs = 12,
	kCommandSetTypeGetSourceFiles2 = 13,
};

enum CommandSetStackFrame {
	kCommandSetStackFrameGetValues = 1,
	kCommandSetStackFrameGetThis = 2,
	kCommandSetStackFrameSetValues = 3
};

enum CommandSetArrayRef {
	kCommandSetArrayRefGetLength = 1,
	kCommandSetArrayRefGetValues = 2,
	kCommandSetArrayRefSetValues = 3,
};

enum CommandSetStringRef {
	kCommandSetStringRefGetValue = 1,
} ;

enum CommandSetObjectRef {
	kCommandSetObjectRefGetType = 1,
	kCommandSetObjectRefGetValues = 2,
	kCommandSetObjectRefIsCollected = 3,
	kCommandSetObjectRefGetAddress = 4,
	kCommandSetObjectRefGetDomain = 5,
	kCommandSetObjectRefSetValues = 6
};

enum ErrorCode {
	kErrorCodeNone = 0,
	kErrorCodeInvalidObject = 20,
	kErrorCodeInvalidFieldId = 25,
	kErrorCodeInvalidFrameId = 30,
	kErrorCodeNotImplemented = 100,
	kErrorCodeNotSuspended = 101,
	kErrorCodeInvalidArgument = 102,
	kErrorCodeUnloaded = 103,
	kErrorCodeNoInvocation = 104,
	kErrorCodeAbsentInformation = 105
};

enum EventKind {
	kEventKindVmStart = 0,
	kEventKindVmDeath = 1,
	kEventKindThreadStart = 2,
	kEventKindThreadDeath = 3,
	kEventKindAppDomainCreate = 4,
	kEventKindAppDomainUnload = 5,
	kEventKindMethodEntry = 6,
	kEventKindMethodExit = 7,
	kEventKindAssemblyLoad = 8,
	kEventKindAssemblyUnload = 9,
	kEventKindBreakpoint = 10,
	kEventKindStep = 11,
	kEventKindTypeLoad = 12,
	kEventKindException = 13,
	kEventKindKeepAlive = 14,
	kEventKindUserBreak = 15,
	kEventKindUserLog = 16,
	kEventKindInvalid = -1
};

enum ModifierKind {
	kModifierKindInvalid = -1,
	kModifierKindCount = 1,
	kModifierKindThreadOnly = 3,
	kModifierKindLocationOnly = 7,
	kModifierKindExceptionOnly = 8,
	kModifierKindStep = 10,
	kModifierKindAssemblyOnly = 11
};

enum StepDepthKind {
	kStepDepthKindInto = 0,
	kStepDepthKindOver = 1,
	kStepDepthKindOut = 2
};

enum StepSizeKind {
	kStepSizeKindMin = 0,
	kStepSizeKindLine = 1
};

struct Modifier
{
	ModifierKind kind;
	union {
		int count; // kModifierKindCount
		Il2CppThread *thread; // kModifierKindThreadOnly
		Il2CppClass *exc_class; // kModifierKindExceptionOnly
		Il2CppAssembly **assemblies; // kModifierKindAssemblyOnly
	} data;
	bool caught;
	bool uncaught; // kModifierKindExceptionOnly

	Modifier() :
		kind(kModifierKindInvalid),
		caught(false),
		uncaught(false)
	{
		data.count = 0;
		data.thread = 0;
		data.exc_class = 0;
		data.assemblies = 0;
	}

	~Modifier()
	{
		if(kind == kModifierKindAssemblyOnly && data.assemblies != 0)
		{
			delete[] data.assemblies;
			data.assemblies = 0;
		}
	}
};

} /* namespace debugger */
} /* namespace il2cpp */
