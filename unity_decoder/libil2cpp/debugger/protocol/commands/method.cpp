#if IL2CPP_DEBUGGER_ENABLED

#include "method.h"

using namespace il2cpp::debugger;

void MethodGetDebugInfoCommand::Reply::WriteContentTo(Buffer &out) const
{
    code_size.WriteTo(out);
    file_name.WriteTo(out);

    out.WriteInt(_offsets.size());

    std::vector<std::pair<int32_t, int32_t> >::const_iterator it = _offsets.begin();

    while (it != _offsets.end())
    {
        out.WriteInt((*it).first);
        out.WriteInt((*it).second);

        ++it;
    }
}

void MethodGetParamInfoCommand::Reply::WriteContentTo(Buffer &out) const
{
    call_convention.WriteTo(out);
    out.WriteInt(_params.size());
    generic_param_count.WriteTo(out);
    return_type.WriteTo(out);

    std::vector<ParamInfo>::const_iterator it = _params.begin();

    while (it != _params.end())
    {
        out.WriteTypeId((*it).type);
        ++it;
    }

    it = _params.begin();

    while (it != _params.end())
    {
        out.WriteString((*it).name);
        ++it;
    }
}

void MethodGetLocalsInfoCommand::Reply::WriteContentTo(Buffer &out) const
{
    out.WriteInt(_locals.size());

    std::vector<LocalInfo>::const_iterator it;

    for (it = _locals.begin(); it != _locals.end(); ++it)
    {
        out.WriteTypeId((Il2CppClass*)(*it).type);
    }

    for (it = _locals.begin(); it != _locals.end(); ++it)
    {
        out.WriteString((*it).name);
    }

    for (it = _locals.begin(); it != _locals.end(); ++it)
    {
        out.WriteInt((*it).begin_scope);
        out.WriteInt((*it).end_scope);
    }
}

#endif
