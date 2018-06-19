#pragma once

#include "common.h"
#include "transport.h"

namespace il2cpp
{
namespace debugger
{

struct TCPDataTransportHandle;

class TCPDataTransport : public IDataTransport
{

public:

	TCPDataTransport(const char *host, int32_t port);
	~TCPDataTransport();

	inline const char *host() const
	{
		return _host;
	}

	inline int32_t port() const
	{
		return _port;
	}

	inline bool connected() const
	{
		return _connected;
	}

	bool Setup();
	bool WaitForClient();
	int32_t Send(const uint8_t *data, size_t length, int flags = 0);
	int32_t Recv(uint8_t *data, size_t length, int flags = 0);
	void Close();

private:

	const char *_host;
	int32_t _port;
	bool _connected;
	TCPDataTransportHandle *_handle;

	DISALLOW_COPY(TCPDataTransport);
};

} /* namespace debugger */
} /* namespace il2cpp */
