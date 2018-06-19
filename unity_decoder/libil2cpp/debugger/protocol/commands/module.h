#pragma once

#include <string>

#include "../../common.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct ModuleGetInfoCommand : public CustomCommand<ModuleGetInfoCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::string> basename;
		Property<std::string> fullname;
		Property<std::string> guid;
		Property<std::string> scope_name;
		Property<Il2CppAssembly*> assembly;

		void WriteContentTo(Buffer &out) const
		{
			basename.WriteTo(out);
			scope_name.WriteTo(out);
			fullname.WriteTo(out);
			guid.WriteTo(out);
			assembly.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Module, GetInfo);

	Property<Il2CppImage*> module;

	void ReadProperties(Buffer &in)
	{
		module.ReadFrom(in);
	}
};

}
}
