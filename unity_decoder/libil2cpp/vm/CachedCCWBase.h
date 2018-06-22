#pragma once

#include "gc/GCHandle.h"
#include "vm/ComObjectBase.h"
#include "utils/TemplateUtils.h"

namespace il2cpp
{
namespace vm
{
// Alright, so the lifetime of this guy is pretty weird
// For a single managed object, the IUnknown of its COM Callable Wrapper must always be the same
// That means that we have to keep the same COM Callable Wrapper alive for an object once we create it
// They are cached in il2cpp::vm::g_CCWCache, which is managed by il2cpp::vm::CCW class
//
// Here comes the tricky part: when a native object has a reference to the COM Callable Wrapper,
// the managed object is not supposed to be garbage collected. However, when no native objects are referencing
// it, it should not prevent the GC from collecting the managed object. We implement this by keeping a GC handle
// on the managed object if our reference count is 1 or more. We acquire it when it gets increased from 0 (this
// is safe because such AddRef can only come when this object is retrieved from CCW Cache) and release the GC
// handle when our reference count gets decreased to 0. Here's a kicker: we don't destroy the COM Callable Wrapper
// when the reference count reaches 0; we instead rely on GC finalizer of the managed object to both remove it from
// CCW cache and also destroy it.
    template<typename TDerived>
    struct NOVTABLE CachedCCWBase : ComObjectBase
    {
    private:
        volatile uint32_t m_RefCount;
        uint32_t m_GCHandle;

    public:
        inline CachedCCWBase(Il2CppObject* obj) :
            ComObjectBase(obj),
            m_RefCount(0), // We do not hold any references upon its creation
            m_GCHandle(0)
        {
            Il2CppStaticAssert(utils::TemplateUtils::IsBaseOf<CachedCCWBase<TDerived>, TDerived>::value);
        }

        virtual uint32_t STDCALL AddRef() IL2CPP_OVERRIDE
        {
            return AddRefImpl();
        }

        virtual uint32_t STDCALL Release() IL2CPP_OVERRIDE
        {
            return ReleaseImpl();
        }

        FORCE_INLINE uint32_t AddRefImpl()
        {
            const uint32_t refCount = Atomic::Increment(&m_RefCount);

            if (refCount == 1)
            {
                IL2CPP_ASSERT(m_GCHandle == 0);
                m_GCHandle = gc::GCHandle::New(GetManagedObjectInline(), false);
            }

            return refCount;
        }

        FORCE_INLINE uint32_t ReleaseImpl()
        {
            const uint32_t count = Atomic::Decrement(&m_RefCount);
            if (count == 0)
            {
                IL2CPP_ASSERT(m_GCHandle != 0);
                gc::GCHandle::Free(m_GCHandle);
                m_GCHandle = 0;
            }

            return count;
        }

        virtual void STDCALL Destroy() IL2CPP_FINAL IL2CPP_OVERRIDE
        {
            IL2CPP_ASSERT(m_RefCount == 0);

            TDerived* instance = static_cast<TDerived*>(this);
            instance->~TDerived();
            utils::Memory::Free(instance);
        }
    };
}
}
