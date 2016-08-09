#pragma once

#include <string>
#include <vector>

#include "../../common.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct MethodGetDeclaringTypeCommand : public CustomCommand<MethodGetDeclaringTypeCommand>
{
	struct Reply : public CustomReply
	{
		Property<Il2CppClass*> type;

		void WriteContentTo(Buffer &out) const
		{
			type.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Method, GetDeclaringType);

	Property<const MethodInfo*> method;

	void ReadProperties(Buffer &in)
	{
		method.ReadFrom(in);
	}
};

struct MethodGetDebugInfoCommand : public CustomCommand<MethodGetDebugInfoCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> code_size;
		Property<std::string> file_name;

		inline std::vector<std::pair<int32_t, int32_t> > &offsets()
		{
			return _offsets;
		}

		void WriteContentTo(Buffer &out) const;

	private:

		std::vector<std::pair<int32_t, int32_t> > _offsets;
	};


	CUSTOM_COMMAND(Method, GetDebugInfo);

	Property<const MethodInfo*> method;

	void ReadProperties(Buffer &in)
	{
		method.ReadFrom(in);
	}
};

struct MethodGetNameCommand : public CustomCommand<MethodGetNameCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::string> name;

		void WriteContentTo(Buffer &out) const
		{
			name.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Method, GetName);

	Property<const MethodInfo*> method;

	void ReadProperties(Buffer &in)
	{
		method.ReadFrom(in);
	}
};

struct MethodGetParamInfoCommand : public CustomCommand<MethodGetParamInfoCommand>
{
	struct Reply : public CustomReply
	{
		struct ParamInfo
		{
			const char *name;
			Il2CppClass *type;
		};

		Property<int32_t> call_convention;
		Property<int32_t> generic_param_count;
		Property<Il2CppClass*> return_type;

		inline std::vector<ParamInfo> &params()
		{
			return _params;
		}

		void WriteContentTo(Buffer &out) const;

	private:

		std::vector<ParamInfo> _params;
	};


	CUSTOM_COMMAND(Method, GetParamInfo);

	Property<const MethodInfo*> method;

	void ReadProperties(Buffer &in)
	{
		method.ReadFrom(in);
	}
};

struct MethodGetBodyCommand : public CustomCommand<MethodGetBodyCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::vector<uint8_t> > bytes;

		void WriteContentTo(Buffer &out) const
		{
			bytes.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Method, GetBody);

	Property<const MethodInfo*> method;

	void ReadProperties(Buffer &in)
	{
		method.ReadFrom(in);
	}
};

struct MethodGetLocalsInfoCommand : public CustomCommand<MethodGetLocalsInfoCommand>
{
	struct Reply : public CustomReply
	{
		struct LocalInfo
		{
			const Il2CppClass *type;
			std::string name;
			uint32_t begin_scope;
			uint32_t end_scope;
		};

		inline std::vector<LocalInfo> &locals()
		{
			return _locals;
		}

		void WriteContentTo(Buffer &out) const;

	private:

		std::vector<LocalInfo> _locals;
	};


	CUSTOM_COMMAND(Method, GetLocalsInfo);

	Property<const MethodInfo*> method;

	void ReadProperties(Buffer &in)
	{
		method.ReadFrom(in);
	}
};

struct MethodGetInfoCommand : public CustomCommand<MethodGetInfoCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> flags;
		Property<int32_t> implementation_flags;
		Property<int32_t> token;

		void WriteContentTo(Buffer &out) const
		{
			flags.WriteTo(out);
			implementation_flags.WriteTo(out);
			token.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Method, GetInfo);

	Property<const MethodInfo*> method;

	void ReadProperties(Buffer &in)
	{
		method.ReadFrom(in);
	}
};

struct MethodResolveTokenCommand : public CustomCommand<MethodResolveTokenCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented");
		}
	};


	CUSTOM_COMMAND(Method, ResolveToken);

	Property<const MethodInfo*> method;

	void ReadProperties(Buffer &in)
	{
		method.ReadFrom(in);
	}
};

}
}
