#if IL2CPP_UNIT_TESTS_ENABLED

#include "il2cpp-api-test-forwarder.h"
#include "agent.h"

using namespace il2cpp::debugger;

const Il2CppApiTestForwarder Il2CppApiTestForwarderAutoClear::zero_forwarder = {};

Il2CppApiTestForwarderAutoClear::Il2CppApiTestForwarderAutoClear()
{
    ClearAllForwardingMethods();
}

Il2CppApiTestForwarderAutoClear::~Il2CppApiTestForwarderAutoClear()
{
    ClearAllForwardingMethods();
}

void Il2CppApiTestForwarderAutoClear::ClearAllForwardingMethods()
{
    Agent::instance().TestForwarder = zero_forwarder;
}

#endif // IL2CPP_UNIT_TESTS_ENABLED
