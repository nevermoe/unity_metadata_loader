#pragma once

#include "common.h"

namespace il2cpp
{
namespace debugger
{

class IDataTransport
{

public:

	IDataTransport() {}
	virtual ~IDataTransport() {}

	virtual bool Setup() = 0;
	virtual bool WaitForClient() = 0;
	virtual int32_t Send(const uint8_t *data, size_t length, int flags = 0) = 0;
	virtual int32_t Recv(uint8_t *data, size_t length, int flags = 0) = 0;
	virtual void Close() = 0;
	
	DISALLOW_COPY(IDataTransport);
};

} /* namespace debugger */
} /* namespace il2cpp */
