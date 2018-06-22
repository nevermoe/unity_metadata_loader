#include "il2cpp-config.h"
#include "object-internals.h"
#include "class-internals.h"
#include "vm/Object.h"
#include "vm/CCW.h"
#include "vm/Class.h"
#include "vm/CachedCCWBase.h"
#include "vm/Exception.h"
#include "vm/MetadataCache.h"
#include "vm/RCW.h"
#include "vm/Runtime.h"
#include "vm/String.h"

namespace il2cpp
{
namespace vm
{
    struct ManagedObject : CachedCCWBase<ManagedObject>
    {
        inline ManagedObject(Il2CppObject* obj) :
            CachedCCWBase<ManagedObject>(obj)
        {
        }

        virtual il2cpp_hresult_t STDCALL QueryInterface(const Il2CppGuid& iid, void** object) IL2CPP_OVERRIDE
        {
            if (::memcmp(&iid, &Il2CppIUnknown::IID, sizeof(Il2CppGuid)) == 0
                || ::memcmp(&iid, &Il2CppIInspectable::IID, sizeof(Il2CppGuid)) == 0
                || ::memcmp(&iid, &Il2CppIAgileObject::IID, sizeof(Il2CppGuid)) == 0)
            {
                *object = GetIdentity();
                AddRefImpl();
                return IL2CPP_S_OK;
            }

            if (::memcmp(&iid, &Il2CppIManagedObjectHolder::IID, sizeof(Il2CppGuid)) == 0)
            {
                *object = static_cast<Il2CppIManagedObjectHolder*>(this);
                AddRefImpl();
                return IL2CPP_S_OK;
            }

            if (::memcmp(&iid, &Il2CppIMarshal::IID, sizeof(Il2CppGuid)) == 0)
            {
                *object = static_cast<Il2CppIMarshal*>(this);
                AddRefImpl();
                return IL2CPP_S_OK;
            }

            *object = NULL;
            return IL2CPP_E_NOINTERFACE;
        }

        virtual il2cpp_hresult_t STDCALL GetIids(uint32_t* iidCount, Il2CppGuid** iids) IL2CPP_OVERRIDE
        {
            *iidCount = 0;
            *iids = NULL;
            return IL2CPP_S_OK;
        }
    };

    Il2CppIUnknown* CCW::CreateCCW(Il2CppObject* obj)
    {
        // check for ccw create function, which is implemented by objects that implement COM or Windows Runtime interfaces
        const Il2CppInteropData* interopData = obj->klass->interopData;
        if (interopData != NULL)
        {
            const CreateCCWFunc createCcw = interopData->createCCWFunction;

            if (createCcw != NULL)
                return createCcw(obj);
        }

        // otherwise create generic ccw object that "only" implements IUnknown, IMarshal, IInspectable, IAgileObject and IManagedObjectHolder interfaces
        void* memory = utils::Memory::Malloc(sizeof(ManagedObject));
        if (memory == NULL)
            Exception::RaiseOutOfMemoryException();
        return static_cast<Il2CppIManagedObjectHolder*>(new(memory)ManagedObject(obj));
    }

    Il2CppException* CCW::GetIPropertyValueInvalidCast(Il2CppObject* value, const char* from, const char* to)
    {
        Il2CppClass* klass = il2cpp::vm::Object::GetClass(value);
        const MethodInfo* toString = il2cpp::vm::Class::GetMethodFromName(klass, "ToString", 0);
        Il2CppString* valueString = (Il2CppString*)il2cpp::vm::Runtime::Invoke(toString, value, NULL, NULL);
        std::string utf8Value = il2cpp::utils::StringUtils::Utf16ToUtf8(il2cpp::utils::StringUtils::GetChars(valueString));
        std::string message = il2cpp::utils::StringUtils::Printf(
                "Object in an IPropertyValue is of type '%s' with value '%s', which cannot be converted to a '%s'.",
                from,
                utf8Value.c_str(),
                to);
        return il2cpp::vm::Exception::GetInvalidCastException(message.c_str());
    }
} /* namespace vm */
} /* namespace il2cpp */
