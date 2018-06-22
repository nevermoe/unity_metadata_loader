#if IL2CPP_DEBUGGER_ENABLED

#include "utils.h"

namespace il2cpp
{
namespace debugger
{
    const char* command_set_to_string(CommandSet command_set)
    {
        switch (command_set)
        {
            case kCommandSetInternalError:  return "INTERNAL_ERROR";
            case kCommandSetVm:             return "VM";
            case kCommandSetObjectRef:      return "OBJECT_REF";
            case kCommandSetStringRef:      return "STRING_REF";
            case kCommandSetThread:         return "THREAD";
            case kCommandSetArrayRef:       return "ARRAY_REF";
            case kCommandSetEventRequest:   return "EVENT_REQUEST";
            case kCommandSetStackFrame:     return "STACK_FRAME";
            case kCommandSetAppDomain:      return "APPDOMAIN";
            case kCommandSetAssembly:       return "ASSEMBLY";
            case kCommandSetMethod:         return "METHOD";
            case kCommandSetType:           return "TYPE";
            case kCommandSetModule:         return "MODULE";
            case kCommandSetEvent:          return "EVENT";
            default:                        return "";
        }
    }

    const char* command_to_string(CommandSet command_set, int command)
    {
        switch (command_set)
        {
            case kCommandSetInternalError:
            {
                switch ((CommandSetInternalError)command)
                {
                    case kCommandSetInternalErrorNotImplemented:    return "CMD_INT_ERR_NOT_IMPL";
                    default:                                    return "";
                }
            }

            case kCommandSetVm:
            {
                switch ((CommandSetVm)command)
                {
                    case kCommandSetVmVersion:              return "CMD_VM_VERSION";
                    case kCommandSetVmAllThreads:           return "CMD_VM_ALL_THREADS";
                    case kCommandSetVmSuspend:              return "CMD_VM_SUSPEND";
                    case kCommandSetVmResume:               return "CMD_VM_RESUME";
                    case kCommandSetVmExit:                 return "CMD_VM_EXIT";
                    case kCommandSetVmDispose:              return "CMD_VM_DISPOSE";
                    case kCommandSetVmInvokeMethod:         return "CMD_VM_INVOKE_METHOD";
                    case kCommandSetVmSetProtocolVersion:   return "CMD_VM_SET_PROTOCOL_VERSION";
                    case kCommandSetVmAbortInvoke:          return "CMD_VM_ABORT_INVOKE";
                    default:                                return "";
                }
            }

            case kCommandSetObjectRef:
            {
                switch ((CommandSetObjectRef)command)
                {
                    case kCommandSetObjectRefGetType:       return "CMD_OBJECT_REF_GET_TYPE";
                    case kCommandSetObjectRefGetValues:     return "CMD_OBJECT_REF_GET_VALUES";
                    case kCommandSetObjectRefIsCollected:   return "CMD_OBJECT_REF_IS_COLLECTED";
                    case kCommandSetObjectRefGetAddress:    return "CMD_OBJECT_REF_GET_ADDRESS";
                    case kCommandSetObjectRefGetDomain:     return "CMD_OBJECT_REF_GET_DOMAIN";
                    case kCommandSetObjectRefSetValues:     return "CMD_OBJECT_REF_SET_VALUES";
                    default:                                return "";
                }
            }

            case kCommandSetStringRef:
            {
                switch ((CommandSetStringRef)command)
                {
                    case kCommandSetStringRefGetValue:  return "CMD_STRING_REF_GET_VALUE";
                    default:                            return "";
                }
            }

            case kCommandSetThread:
            {
                switch ((CommandSetThread)command)
                {
                    case kCommandSetThreadGetName:          return "CMD_THREAD_GET_NAME";
                    case kCommandSetThreadGetFrameInfo: return "CMD_THREAD_GET_FRAME_INFO";
                    case kCommandSetThreadGetState:     return "CMD_THREAD_GET_STATE";
                    case kCommandSetThreadGetInfo:          return "CMD_THREAD_GET_INFO";
                    case kCommandSetThreadGetId:            return "CMD_THREAD_GET_ID";
                    default:                            return "";
                }
            }

            case kCommandSetArrayRef:
            {
                switch ((CommandSetArrayRef)command)
                {
                    case kCommandSetArrayRefGetLength:  return "CMD_ARRAY_REF_GET_LENGTH";
                    case kCommandSetArrayRefGetValues:  return "CMD_ARRAY_REF_GET_VALUES";
                    case kCommandSetArrayRefSetValues:  return "CMD_ARRAY_REF_SET_VALUES";
                    default:                            return "";
                }
            }

            case kCommandSetEventRequest:
            {
                switch ((CommandSetEvent)command)
                {
                    case kCommandSetEventRequestSet:                    return "CMD_EVENT_REQUEST_SET";
                    case kCommandSetEventRequestClear:                  return "CMD_EVENT_REQUEST_CLEAR";
                    case kCommandSetEventRequestClearAllBreakpoints:    return "CMD_EVENT_REQUEST_CLEAR_ALL_BREAKPOINTS";
                    default:                                        return "";
                }
            }

            case kCommandSetStackFrame:
            {
                switch ((CommandSetStackFrame)command)
                {
                    case kCommandSetStackFrameGetValues:    return "CMD_STACK_FRAME_GET_VALUES";
                    case kCommandSetStackFrameGetThis:      return "CMD_STACK_FRAME_GET_THIS";
                    case kCommandSetStackFrameSetValues:    return "CMD_STACK_FRAME_SET_VALUES";
                    default:                            return "";
                }
            }

            case kCommandSetAppDomain:
            {
                switch ((CommandSetAppDomain)command)
                {
                    case kCommandSetAppDomainGetRootDomain: return "CMD_APPDOMAIN_GET_ROOT_DOMAIN";
                    case kCommandSetAppDomainGetFriendlyName:   return "CMD_APPDOMAIN_GET_FRIENDLY_NAME";
                    case kCommandSetAppDomainGetAssemblies: return "CMD_APPDOMAIN_GET_ASSEMBLIES";
                    case kCommandSetAppDomainGetEntryAssembly:  return "CMD_APPDOMAIN_GET_ENTRY_ASSEMBLY";
                    case kCommandSetAppDomainCreateString:      return "CMD_APPDOMAIN_CREATE_STRING";
                    case kCommandSetAppDomainGetCorlib:     return "CMD_APPDOMAIN_GET_CORLIB";
                    case kCommandSetAppDomainCreateBoxedValue:  return "CMD_APPDOMAIN_CREATE_BOXED_VALUE";
                    default:                                return "";
                }
            }

            case kCommandSetAssembly:
            {
                switch ((CommandSetAssembly)command)
                {
                    case kCommandSetAssemblyGetLocation:        return "CMD_ASSEMBLY_GET_LOCATION";
                    case kCommandSetAssemblyGetEntryPoint:      return "CMD_ASSEMBLY_GET_ENTRY_POINT";
                    case kCommandSetAssemblyGetManifestModule:  return "CMD_ASSEMBLY_GET_MANIFEST_MODULE";
                    case kCommandSetAssemblyGetObject:          return "CMD_ASSEMBLY_GET_OBJECT";
                    case kCommandSetAssemblyGetType:            return "CMD_ASSEMBLY_GET_TYPE";
                    case kCommandSetAssemblyGetName:            return "CMD_ASSEMBLY_GET_NAME";
                    default:                                return "";
                }
            }

            case kCommandSetMethod:
            {
                switch ((CommandSetMethod)command)
                {
                    case kCommandSetMethodGetName:              return "CMD_METHOD_GET_NAME";
                    case kCommandSetMethodGetDeclaringType: return "CMD_METHOD_GET_DECLARING_TYPE";
                    case kCommandSetMethodGetDebugInfo:     return "CMD_METHOD_GET_DEBUG_INFO";
                    case kCommandSetMethodGetParamInfo:     return "CMD_METHOD_GET_PARAM_INFO";
                    case kCommandSetMethodGetLocalsInfo:        return "CMD_METHOD_GET_LOCALS_INFO";
                    case kCommandSetMethodGetInfo:              return "CMD_METHOD_GET_INFO";
                    case kCommandSetMethodGetBody:              return "CMD_METHOD_GET_BODY";
                    case kCommandSetMethodResolveToken:     return "CMD_METHOD_RESOLVE_TOKEN";
                    default:                                return "";
                }
            }

            case kCommandSetType:
            {
                switch ((CommandSetType)command)
                {
                    case kCommandSetTypeGetInfo:            return "CMD_TYPE_GET_INFO";
                    case kCommandSetTypeGetMethods:     return "CMD_TYPE_GET_METHODS";
                    case kCommandSetTypeGetFields:          return "CMD_TYPE_GET_FIELDS";
                    case kCommandSetTypeGetValues:          return "CMD_TYPE_GET_VALUES";
                    case kCommandSetTypeGetObject:          return "CMD_TYPE_GET_OBJECT";
                    case kCommandSetTypeGetSourceFiles: return "CMD_TYPE_GET_SOURCE_FILES";
                    case kCommandSetTypeSetValues:          return "CMD_TYPE_SET_VALUES";
                    case kCommandSetTypeIsAssignableFrom:   return "CMD_TYPE_IS_ASSIGNABLE_FROM";
                    case kCommandSetTypeGetProperties:      return "CMD_TYPE_GET_PROPERTIES";
                    case kCommandSetTypeGetCAttrs:          return "CMD_TYPE_GET_CATTRS";
                    case kCommandSetTypeGetFieldCAttrs: return "CMD_TYPE_GET_FIELD_CATTRS";
                    case kCommandSetTypeGetPropertyCAttrs:  return "CMD_TYPE_GET_PROPERTY_CATTRS";
                    case kCommandSetTypeGetSourceFiles2:    return "CMD_TYPE_GET_SOURCE_FILES_2";
                    default:                            return "";
                }
            }

            case kCommandSetModule:
            {
                switch ((CommandSetModule)command)
                {
                    case kCommandSetModuleGetInfo:          return "CMD_MODULE_GET_INFO";
                    default:                            return "";
                }
            }

            case kCommandSetEvent:
            {
                switch ((CommandSetEvent)command)
                {
                    case kCommandSetEventComposite: return "CMD_COMPOSITE";
                    default:                        return "";
                }
            }

            default:
                return "";
        }
    }

    const char* event_to_string(EventKind evt)
    {
        switch (evt)
        {
            case kEventKindVmStart:         return "VM_START";
            case kEventKindVmDeath:         return "VM_DEATH";
            case kEventKindThreadStart:     return "THREAD_START";
            case kEventKindThreadDeath:     return "THREAD_DEATH";
            case kEventKindAppDomainCreate: return "APPDOMAIN_CREATE";
            case kEventKindAppDomainUnload: return "APPDOMAIN_UNLOAD";
            case kEventKindMethodEntry:     return "METHOD_ENTRY";
            case kEventKindMethodExit:      return "METHOD_EXIT";
            case kEventKindAssemblyLoad:    return "ASSEMBLY_LOAD";
            case kEventKindAssemblyUnload:  return "ASSEMBLY_UNLOAD";
            case kEventKindBreakpoint:      return "BREAKPOINT";
            case kEventKindStep:            return "STEP";
            case kEventKindTypeLoad:        return "TYPE_LOAD";
            case kEventKindException:       return "EXCEPTION";
            default:                        return "INVALID";
        }
    }
} /* namespace debugger */
} /* namespace il2cpp */

#endif
