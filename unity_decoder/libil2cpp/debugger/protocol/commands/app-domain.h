#pragma once

#include "../../common.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct AppDomainGetRootDomainCommand : public CustomCommand<AppDomainGetRootDomainCommand>
{
	struct Reply : public CustomReply
	{
		Property<Il2CppDomain*> domain;

		void WriteContentTo(Buffer &out) const
		{
			domain.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(AppDomain, GetRootDomain);
};

struct AppDomainGetCorlibCommand : public CustomCommand<AppDomainGetCorlibCommand>
{
	struct Reply : public CustomReply
	{
		Property<Il2CppAssembly*> assembly;

		void WriteContentTo(Buffer &out) const
		{
			assembly.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(AppDomain, GetCorlib);

	Property<Il2CppDomain*> domain;

	void ReadProperties(Buffer &in)
	{
		domain.ReadFrom(in);
	}
};

struct AppDomainGetFriendlyNameCommand : public CustomCommand<AppDomainGetFriendlyNameCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::string> friendly_name;

		void WriteContentTo(Buffer &out) const
		{
			friendly_name.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(AppDomain, GetFriendlyName);

	Property<Il2CppDomain*> domain;

	void ReadProperties(Buffer &in)
	{
		domain.ReadFrom(in);
	}
};

struct AppDomainCreateStringCommand : public CustomCommand<AppDomainCreateStringCommand>
{
	struct Reply : public CustomReply
	{
		Property<Il2CppObject*> object;

		void WriteContentTo(Buffer &out) const
		{
			object.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(AppDomain, CreateString);

	Property<Il2CppDomain*> domain;
	Property<std::string> string;

	void ReadProperties(Buffer &in)
	{
		domain.ReadFrom(in);
		string.ReadFrom(in);
	}
};

struct AppDomainGetEntryAssemblyCommand : public CustomCommand<AppDomainGetEntryAssemblyCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented");
		}
	};


	CUSTOM_COMMAND(AppDomain, GetEntryAssembly);

	Property<Il2CppDomain*> domain;

	void ReadProperties(Buffer &in)
	{
		domain.ReadFrom(in);
	}
};

struct AppDomainCreateBoxedValueCommand : public CustomCommand<AppDomainCreateBoxedValueCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented");
		}
	};


	CUSTOM_COMMAND(AppDomain, CreateBoxedValue);

	Property<Il2CppDomain*> domain;

	void ReadProperties(Buffer &in)
	{
		domain.ReadFrom(in);
	}
};

struct AppDomainGetAssembliesCommand : public CustomCommand<AppDomainGetAssembliesCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> number_of_assemblies;
		Property<std::vector<Il2CppAssembly*> > assemblies;

		void WriteContentTo(Buffer &out) const
		{
			number_of_assemblies.WriteTo(out);
			assemblies.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(AppDomain, GetAssemblies);

	Property<Il2CppDomain*> domain;

	void ReadProperties(Buffer &in)
	{
		domain.ReadFrom(in);
	}
};

}
}
