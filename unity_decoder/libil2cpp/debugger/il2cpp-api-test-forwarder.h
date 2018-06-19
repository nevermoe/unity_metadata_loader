#pragma once

#include <object-internals.h>

namespace il2cpp
{
namespace debugger
{

#if IL2CPP_UNIT_TESTS_ENABLED

struct Il2CppApiTestForwarder
{
	Il2CppDomain* (*il2cpp_domain_get)();
	Il2CppThread* (*il2cpp_thread_current)();
	Il2CppThread** (*il2cpp_thread_get_all_attached_threads)(size_t *size);
	bool (*il2cpp_is_vm_thread)(Il2CppThread *thread);
	Il2CppObject* (*il2cpp_runtime_invoke)(MethodInfo *method, void *obj, void **params, Il2CppObject **exc);
	const Il2CppType* (*il2cpp_method_get_return_type)(MethodInfo* method);
	void* (*il2cpp_object_unbox)(Il2CppObject* obj);
};

class Il2CppApiTestForwarderAutoClear
{
public:
	Il2CppApiTestForwarderAutoClear();
	~Il2CppApiTestForwarderAutoClear();

private:
	void ClearAllForwardingMethods();
	static const Il2CppApiTestForwarder zero_forwarder;
};

#define CALL_IL2CPP_API(x) Agent::instance().TestForwarder.x
#define ENABLE_IL2CPP_API_METHOD_FORWARDING Il2CppApiTestForwarderAutoClear __define_ENABLE_IL2CPP_API_METHOD_FORWARDING_before_forwarding_any_methods_consider_using_Il2CppApiForwardingTestFixture;
#define FORWARD_IL2CPP_API_METHOD_UNSAFE(method, target) Agent::instance().TestForwarder.method = target;
#define FORWARD_IL2CPP_API_METHOD(method, target)\
	__define_ENABLE_IL2CPP_API_METHOD_FORWARDING_before_forwarding_any_methods_consider_using_Il2CppApiForwardingTestFixture;\
	FORWARD_IL2CPP_API_METHOD_UNSAFE(method, target)

#else

#define CALL_IL2CPP_API(x) x
#define ENABLE_IL2CPP_API_METHOD_FORWARDING
#define FORWARD_IL2CPP_API_METHOD_UNSAFE(method, target)
#define FORWARD_IL2CPP_API_METHOD(method, target)

#endif // IL2CPP_UNIT_TESTS_ENABLED

} /* namspace debugger */
} /* namespace il2cpp */
