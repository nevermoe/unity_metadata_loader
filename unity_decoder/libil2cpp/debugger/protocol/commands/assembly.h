#pragma once

#include "../../common.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct AssemblyGetTypeCommand : public CustomCommand<AssemblyGetTypeCommand>
{
	struct Reply : public CustomReply
	{
		Property<Il2CppClass*> type;

		void WriteContentTo(Buffer &out) const
		{
			type.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Assembly, GetType);

	Property<Il2CppAssembly*> assembly;
	Property<std::string> name;
	Property<bool> ignore_case;

	void ReadProperties(Buffer &in)
	{
		assembly.ReadFrom(in);
		name.ReadFrom(in);
		ignore_case.ReadFrom(in);
	}
};

struct AssemblyGetObjectCommand : public CustomCommand<AssemblyGetObjectCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented");
		}
	};


	CUSTOM_COMMAND(Assembly, GetObject);
};

struct AssemblyGetEntryPointCommand : public CustomCommand<AssemblyGetEntryPointCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented");
		}
	};


	CUSTOM_COMMAND(Assembly, GetEntryPoint);
};

struct AssemblyGetManifestModuleCommand : public CustomCommand<AssemblyGetManifestModuleCommand>
{
	struct Reply : public CustomReply
	{
		Property<const Il2CppImage*> module;

		void WriteContentTo(Buffer &out) const
		{
			module.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Assembly, GetManifestModule);

	Property<Il2CppAssembly*> assembly;

	void ReadProperties(Buffer &in)
	{
		assembly.ReadFrom(in);
	}
};

struct AssemblyGetNameCommand : public CustomCommand<AssemblyGetNameCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::string> name;

		void WriteContentTo(Buffer &out) const
		{
			name.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Assembly, GetName);

	Property<Il2CppAssembly*> assembly;

	void ReadProperties(Buffer &in)
	{
		assembly.ReadFrom(in);
	}
};

struct AssemblyGetLocationCommand : public CustomCommand<AssemblyGetLocationCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::string> location;

		void WriteContentTo(Buffer &out) const
		{
			location.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Assembly, GetLocation);

	Property<Il2CppAssembly*> assembly;

	void ReadProperties(Buffer &in)
	{
		assembly.ReadFrom(in);
	}
};

}
}
