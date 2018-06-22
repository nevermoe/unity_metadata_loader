#if IL2CPP_DEBUGGER_ENABLED

#include "events.h"

#include "../../il2cpp-api.h"

#include "../agent.h"
#include "../id-bank.h"
#include "../data-buffer.h"

using namespace il2cpp::debugger;

VmStartEvent::VmStartEvent()
{
}

VmStartEvent::VmStartEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void VmStartEvent::WriteContentTo(Buffer &out) const
{
    out.WritePtrId(DomainId(il2cpp_domain_get()));
}

VmDeathEvent::VmDeathEvent()
{
}

VmDeathEvent::VmDeathEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void VmDeathEvent::WriteContentTo(Buffer &out) const
{
}

UserBreakEvent::UserBreakEvent()
{
}

UserBreakEvent::UserBreakEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void UserBreakEvent::WriteContentTo(Buffer &out) const
{
}

AppDomainCreateEvent::AppDomainCreateEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void AppDomainCreateEvent::WriteContentTo(Buffer &out) const
{
    domain.WriteTo(out);
}

AssemblyLoadEvent::AssemblyLoadEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void AssemblyLoadEvent::WriteContentTo(Buffer &out) const
{
    assembly.WriteTo(out);
}

TypeLoadEvent::TypeLoadEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void TypeLoadEvent::WriteContentTo(Buffer &out) const
{
    type.WriteTo(out);
}

BreakPointEvent::BreakPointEvent()
{
}

BreakPointEvent::BreakPointEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void BreakPointEvent::WriteContentTo(Buffer &out) const
{
    method.WriteTo(out);
    il_offset.WriteTo(out);
}

StepEvent::StepEvent()
{
}

StepEvent::StepEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void StepEvent::WriteContentTo(Buffer &out) const
{
    method.WriteTo(out);
    il_offset.WriteTo(out);
}

ThreadStartEvent::ThreadStartEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void ThreadStartEvent::WriteContentTo(Buffer &out) const
{
}

ThreadDeathEvent::ThreadDeathEvent(Il2CppThread *thread) :
    Event(thread)
{
}

void ThreadDeathEvent::WriteContentTo(Buffer &out) const
{
}

#endif
