#pragma once

#include <set>
#include <string>
#include <vector>

#include "../../common.h"
#include "../../variant.h"
#include "../../../class-internals.h"
#include "../../../il2cpp-api.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct TypeGetInfoCommand : public CustomCommand<TypeGetInfoCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::string> ns;
		Property<std::string> name;
		Property<std::string> fullname;
		Property<Il2CppAssembly*> assembly;
		Property<Il2CppImage*> module;
		Property<Il2CppClass*> parent_type;
		Property<Il2CppClass*> underlying_type;
		Property<int32_t> type_token;
		Property<uint8_t> rank;
		Property<int32_t> flags;
		Property<uint8_t> by_val_flags;
		Property<std::vector<Il2CppClass*> > nested_types;

		void WriteContentTo(Buffer &out) const
		{
			ns.WriteTo(out);
			name.WriteTo(out);
			fullname.WriteTo(out);
			assembly.WriteTo(out);
			module.WriteTo(out);
			parent_type.WriteTo(out);
			underlying_type.WriteTo(out);
			type_token.WriteTo(out);
			rank.WriteTo(out);
			flags.WriteTo(out);
			by_val_flags.WriteTo(out);
			nested_types.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Type, GetInfo);

	Property<Il2CppClass*> type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
	}
};

struct TypeGetSourceFilesCommand : public CustomCommand<TypeGetSourceFilesCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::set<std::string> > source_files;

		void WriteContentTo(Buffer &out) const
		{
			source_files.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Type, GetSourceFiles);

	Property<Il2CppClass*> type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
	}
};

struct TypeGetPropertiesCommand : public CustomCommand<TypeGetPropertiesCommand>
{
	struct Reply : public CustomReply
	{
		std::vector<const PropertyInfo*> properties;

		void WriteContentTo(Buffer &out) const
		{
			out.WriteInt(properties.size());

			std::vector<const PropertyInfo*>::const_iterator it = properties.begin();
			while(it != properties.end())
			{
				const PropertyInfo *prop = *it;
				out.WritePropertyId(prop);
				out.WriteString(prop->name);
				out.WriteMethodId(prop->get);
				out.WriteMethodId(prop->set);
				out.WriteInt(prop->attrs);

				++it;
			}
		}
	};


	CUSTOM_COMMAND(Type, GetProperties);

	Property<Il2CppClass*> type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
	}
};

struct TypeGetSourceFiles2Command : public CustomCommand<TypeGetSourceFiles2Command>
{
	struct Reply : public CustomReply
	{
		Property<std::set<std::string> > source_files;

		void WriteContentTo(Buffer &out) const
		{
			source_files.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Type, GetSourceFiles2);

	Property<Il2CppClass*> type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
	}
};

struct TypeGetPropertyCAttrsCommand : public CustomCommand<TypeGetPropertyCAttrsCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> fake_count;

		void WriteContentTo(Buffer &out) const
		{
			fake_count.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Type, GetPropertyCAttrs);

	Property<Il2CppClass*> type;
	Property<Il2CppClass*> attr_type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
		attr_type.ReadFrom(in);
	}
};

struct TypeGetFieldCAttrsCommand : public CustomCommand<TypeGetFieldCAttrsCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> fake_count;

		void WriteContentTo(Buffer &out) const
		{
			fake_count.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Type, GetFieldCAttrs);

	Property<Il2CppClass*> type;
	Property<Il2CppClass*> attr_type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
		attr_type.ReadFrom(in);
	}
};

struct TypeIsAssignableFromCommand : public CustomCommand<TypeIsAssignableFromCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented!");
		}
	};


	CUSTOM_COMMAND(Type, IsAssignableFrom);

	Property<Il2CppClass*> type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
	}
};

struct TypeGetCAttrsCommand : public CustomCommand<TypeGetCAttrsCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> fake_count;

		void WriteContentTo(Buffer &out) const
		{
			fake_count.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Type, GetCAttrs);

	Property<Il2CppClass*> type;
	Property<Il2CppClass*> attr_type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
		attr_type.ReadFrom(in);
	}
};

struct TypeGetValuesCommand : public CustomCommand<TypeGetValuesCommand>
{
	struct Reply : public CustomReply
	{
		inline std::vector<Variant> &values()
		{
			return _values;
		}

		void WriteContentTo(Buffer &out) const;

	private:

		std::vector<Variant> _values;
	};


	CUSTOM_COMMAND(Type, GetValues);

	Property<Il2CppClass*> type;
	Property<std::vector<FieldInfo*> > fields;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
		fields.ReadFrom(in);
	}
};

struct TypeGetObjectCommand : public CustomCommand<TypeGetObjectCommand>
{
	struct Reply : public CustomReply
	{
		Property<Il2CppObject*> object;

		void WriteContentTo(Buffer &out) const
		{
			object.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Type, GetObject);

	Property<Il2CppClass*> type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
	}
};

struct TypeGetFieldsCommand : public CustomCommand<TypeGetFieldsCommand>
{
	struct Reply : public CustomReply
	{
		std::vector<FieldInfo*> fields;

		void WriteContentTo(Buffer &out) const
		{
			out.WriteInt(fields.size());

			std::vector<FieldInfo*>::const_iterator it = fields.begin();
			while(it != fields.end())
			{
				FieldInfo *field = *it;
				out.WriteFieldId(field);
				out.WriteString(field->name);
				out.WriteTypeId(il2cpp_class_from_il2cpp_type(field->type));
				out.WriteInt(field->type->attrs);
				++it;
			}
		}
	};


	CUSTOM_COMMAND(Type, GetFields);

	Property<Il2CppClass*> type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
	}
};

struct TypeSetValuesCommand : public CustomCommand<TypeSetValuesCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented!");
		}
	};


	CUSTOM_COMMAND(Type, SetValues);

	Property<Il2CppClass*> type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
	}
};

struct TypeGetMethodsCommand : public CustomCommand<TypeGetMethodsCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::vector<const MethodInfo*> > methods;

		void WriteContentTo(Buffer &out) const
		{
			methods.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Type, GetMethods);

	Property<Il2CppClass*> type;

	void ReadProperties(Buffer &in)
	{
		type.ReadFrom(in);
	}
};

}
}
