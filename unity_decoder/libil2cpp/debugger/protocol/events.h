#pragma once

#include "../common.h"
#include "../meta.h"
#include "protocol.h"

namespace il2cpp
{
namespace debugger
{

class Buffer;
class Command;

class VmStartEvent : public Event
{

public:

	VmStartEvent();
	explicit VmStartEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindVmStart;
	}

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(VmStartEvent);
};

class VmDeathEvent : public Event
{

public:

	VmDeathEvent();
	explicit VmDeathEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindVmDeath;
	}

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(VmDeathEvent);
};

class UserBreakEvent : public Event
{

public:

	UserBreakEvent();
	explicit UserBreakEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindUserBreak;
	}

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(UserBreakEvent);
};

class AppDomainCreateEvent : public Event
{

public:

	AppDomainCreateEvent();
	explicit AppDomainCreateEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindAppDomainCreate;
	}

	Property<Il2CppDomain*> domain;

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(AppDomainCreateEvent);
};

class AssemblyLoadEvent : public Event
{

public:

	AssemblyLoadEvent();
	explicit AssemblyLoadEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindAssemblyLoad;
	}

	Property<Il2CppAssembly*> assembly;

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(AssemblyLoadEvent);
};

class TypeLoadEvent : public Event
{

public:

	TypeLoadEvent();
	explicit TypeLoadEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindTypeLoad;
	}

	Property<Il2CppClass*> type;

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(TypeLoadEvent);
};

class BreakPointEvent : public Event
{

public:

	BreakPointEvent();
	explicit BreakPointEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindBreakpoint;
	}

	Property<const MethodInfo*> method;
	Property<uint64_t> il_offset;

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(BreakPointEvent);
};

class StepEvent : public Event
{

public:

	StepEvent();
	explicit StepEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindStep;
	}

	Property<const MethodInfo*> method;
	Property<uint64_t> il_offset;

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(StepEvent);
};


class ThreadStartEvent : public Event
{

public:

	ThreadStartEvent();
	explicit ThreadStartEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindThreadStart;
	}

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(ThreadStartEvent);
};


class ThreadDeathEvent : public Event
{

public:

	ThreadDeathEvent();
	explicit ThreadDeathEvent(Il2CppThread *thread);

	inline EventKind kind() const
	{
		return kEventKindThreadDeath;
	}

protected:
	
	void WriteContentTo(Buffer &out) const;

	DISALLOW_COPY(ThreadDeathEvent);
};

} /* namespace debugger */
} /* namespace il2cpp */
