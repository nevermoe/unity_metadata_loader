#pragma once

#include "os/Win32/WindowsHeaders.h"
#include <windows.foundation.collections.h>
#include <wrl.h>

namespace il2cpp
{
namespace winrt
{
    template<typename T>
    class SynchronousOperation : Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::WinRtClassicComMix>, ABI::Windows::Foundation::IAsyncOperationCompletedHandler<T> >
    {
    private:
        HANDLE m_Event;
        HRESULT m_HR;
        T m_Result;

    public:
        inline SynchronousOperation(ABI::Windows::Foundation::IAsyncOperation<T>* op)
        {
            m_Event = CreateEventExW(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
            Assert(m_Event);

            auto hr = op->put_Completed(this);
            Assert(SUCCEEDED(hr));
        }

        inline ~SynchronousOperation()
        {
            CloseHandle(m_Event);
        }

        HRESULT GetResults(T* result)
        {
            auto waitResult = WaitForSingleObjectEx(m_Event, INFINITE, FALSE);

            if (waitResult != WAIT_OBJECT_0)
                return E_FAIL;

            if (FAILED(m_HR))
                return m_HR;

            *result = m_Result;
            return S_OK;
        }

    private:
        virtual HRESULT STDMETHODCALLTYPE Invoke(ABI::Windows::Foundation::IAsyncOperation<T>* asyncInfo, ABI::Windows::Foundation::AsyncStatus status) override
        {
            m_HR = asyncInfo->GetResults(&m_Result);
            SetEvent(m_Event);
            return S_OK;
        }
    };
}
}
