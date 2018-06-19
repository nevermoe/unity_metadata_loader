#pragma once

#include <cassert>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>

#include "il2cpp-config.h"

#include "../os/Thread.h"

struct Il2CppAssembly;
struct Il2CppClass;
struct Il2CppObject;
struct Il2CppImage;
struct Il2CppDomain;
struct MethodInfo;
struct FieldInfo;
struct PropertyInfo;
struct Il2CppThread;
struct Il2CppType;

extern bool is_debugger_thread();

#define DISALLOW_COPY(t) \
	private: \
		t(const t&); \
		t& operator=(const t&);

#if IL2CPP_DEBUGGER_LOG
	#define LOG(msg) \
		{\
			std::cout << "[debugger:"<< il2cpp::os::Thread::CurrentThreadId() << "/" << (is_debugger_thread() ? "dbg" : "vm") << "] " << msg << std::endl; \
		}
#else
	#define LOG(msg) do{} while(0)
#endif

enum SuspendPolicy {
	kSuspendPolicyNone = 0,
	kSuspendPolicyEventThread = 1,
	kSuspendPolicyAll = 2
};
