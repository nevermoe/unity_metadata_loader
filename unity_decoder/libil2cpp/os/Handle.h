#pragma once

#include "utils/NonCopyable.h"

namespace il2cpp
{
namespace os
{
	
class Handle : public il2cpp::utils::NonCopyable
{
public:
	virtual ~Handle () { };
	virtual bool Wait () = 0;
	virtual bool Wait (uint32_t ms) = 0;
	virtual void Signal () = 0;
};

}
}
