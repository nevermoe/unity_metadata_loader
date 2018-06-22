#if IL2CPP_DEBUGGER_ENABLED

#include "event-request.h"

using namespace il2cpp::debugger;

void EventRequestSetCommand::ReadProperties(Buffer &in)
{
    _kind = in.ReadEnum<EventKind>();
    _suspend_policy = in.ReadEnum<SuspendPolicy>();

    const int32_t nmodifiers = in.ReadByte();

    for (int i = 0; i < nmodifiers; ++i)
    {
        Modifier *modifier = new Modifier();

        modifier->kind = in.ReadEnum<ModifierKind>();

        switch (modifier->kind)
        {
            case kModifierKindCount:
                modifier->data.count = in.ReadInt();
                break;

            case kModifierKindLocationOnly:
                IL2CPP_ASSERT(method() == 0 && "Duplicated kModifierKindLocationOnly modifier!");

                method.ReadFrom(in);
                location.ReadFrom(in);
                break;

            case kModifierKindStep:
                IL2CPP_ASSERT(thread() == 0 && "Duplicated kModifierKindStep modifier!");

                thread.ReadFrom(in);
                size.ReadFrom(in);
                depth.ReadFrom(in);
                break;

            case kModifierKindThreadOnly:
                IL2CPP_ASSERT(modifier->data.thread == 0 && "Duplicated kModifierKindThreadOnly modifier!");

                modifier->data.thread = in.ReadThread();
                break;

            case kModifierKindExceptionOnly:
                modifier->data.exc_class = in.ReadType();
                modifier->caught = in.ReadBool();
                modifier->uncaught = in.ReadBool();

                // DEBUG(1, fprintf (log_file, "[dbg] \tEXCEPTION_ONLY filter (%s%s%s).\n", exc_class ? exc_class->name : "all", req->modifiers [i].caught ? ", caught" : "", req->modifiers [i].uncaught ? ", uncaught" : ""));
                if (modifier->data.exc_class != 0)
                {
                    /*
                    if (!mono_class_is_assignable_from (mono_defaults.exception_class, exc_class)) {
                        g_free (req);
                        return ERR_INVALID_ARGUMENT;
                    }*/
                }

                break;

            case kModifierKindAssemblyOnly:
            {
                const int32_t count = in.ReadInt();

                modifier->data.assemblies = new Il2CppAssembly*[count];

                for (int32_t i = 0; i < count; ++i)
                    modifier->data.assemblies[i] = in.ReadAssembly();
            }
            break;

            default:
                LOG("Not implemented EventRequestSetCommand+??(" << modifier->kind << ")");
                exit(1);
        }

        _modifiers.push_back(modifier);
    }
}

EventRequestSetCommand::~EventRequestSetCommand()
{
    std::vector<Modifier*>::iterator it = _modifiers.begin();
    while (it != _modifiers.end())
    {
        delete *it;
        ++it;
    }

    _modifiers.clear();
}

void EventRequestClearCommand::ReadProperties(Buffer &in)
{
    event_kind.ReadFrom(in);
    request_id.ReadFrom(in);
}

#endif
