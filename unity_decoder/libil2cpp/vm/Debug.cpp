#include "il2cpp-config.h"

#if IL2CPP_DEBUGGER_ENABLED

#include <assert.h>

#include "../class-internals.h"
#include "vm/Object.h"
#include "vm/Debug.h"
#include "vm/StackTrace.h"
#include "vm/Type.h"

namespace il2cpp
{
namespace vm
{
    const Il2CppDebugDocument *Debug::GetDocument(const Il2CppDebugTypeInfo *info)
    {
        return info->document;
    }

    const char *Debug::DocumentGetFilename(const Il2CppDebugDocument *document)
    {
        return document->filename;
    }

    const char *Debug::DocumentGetDirectory(const Il2CppDebugDocument *document)
    {
        return document->directory;
    }

    const Il2CppDebugDocument* Debug::GetDocument(const Il2CppDebugMethodInfo* info)
    {
        return info->document;
    }

    const int32_t* Debug::GetOffsetTable(const Il2CppDebugMethodInfo* info)
    {
        return info->offsets_table;
    }

    size_t Debug::GetCodeSize(const Il2CppDebugMethodInfo* info)
    {
        return info->code_size;
    }

    void Debug::UpdateFrameIlOffset(int32_t il_offset)
    {
        IL2CPP_ASSERT(0 && "Not implemented");
    }

    const Il2CppDebugLocalsInfo **Debug::GetLocalsInfo(const Il2CppDebugMethodInfo* info)
    {
        return info->locals;
    }

    const Il2CppClass *Debug::GetType(const Il2CppDebugLocalsInfo *info)
    {
        return info->type;
    }

    const char *Debug::GetName(const Il2CppDebugLocalsInfo *info)
    {
        return info->name;
    }

    uint32_t Debug::GetStartOffset(const Il2CppDebugLocalsInfo *info)
    {
        return info->start_offset;
    }

    uint32_t Debug::GetEndOffset(const Il2CppDebugLocalsInfo *info)
    {
        return info->end_offset;
    }

    Il2CppObject *Debug::GetParamValue(const Il2CppStackFrameInfo *info, uint32_t position)
    {
        IL2CPP_ASSERT(info != 0 && "Debug::GetParamValue cannot be invoked on a null Il2CppDebugMethodInfo.");
        IL2CPP_ASSERT(position < info->method->parameters_count && "Debug::GetParamValue was invoked with an outside of range position");

        Il2CppClass *param_type = Type::GetClass(info->method->parameters[position].parameter_type);
        void *param_addr = info->params[position];

        if (param_type->valuetype)
            return Object::Box(param_type, param_addr);

        return static_cast<Il2CppObject*>(param_addr);
    }

    Il2CppObject *Debug::GetLocalValue(const Il2CppStackFrameInfo *info, uint32_t position)
    {
        IL2CPP_ASSERT(info != 0 && "Debug::GetLocalValue cannot be invoked on a null Il2CppDebugMethodInfo.");
        IL2CPP_ASSERT(position < info->locals_count && "Debug::GetLocalValue was invoked with an outside of range position");

        Il2CppClass *local_type = info->method->debug_info->locals[position]->type;
        void *local_addr = info->locals[position];

        if (local_type->valuetype)
            return Object::Box(local_type, local_addr);

        return static_cast<Il2CppObject*>(local_addr);
    }

    void *Debug::GetBreakpointDataAt(const Il2CppDebugMethodInfo* info, int64_t uid, int32_t offset)
    {
        IL2CPP_ASSERT(info != 0 && "Debug::GetBreakpointDataAt cannot be invoked on a null Il2CppDebugMethodInfo.");
        IL2CPP_ASSERT(offset < info->code_size && "Debug::GetBreakpointDataAt invoked with an invalid offset.");

        if (uid < 0)
            return NULL;

        SequencePointRecord *record = &info->sequence_points[uid];

        IL2CPP_ASSERT(record->start_offset <= offset && record->end_offset >= offset && "Debug::GetBreakpointDataAt invoked with an invalid offset (or sequnce point not matching).");

        return record->user_data;
    }

    void Debug::SetBreakpointDataAt(const Il2CppDebugMethodInfo* info, uint64_t location, void *data)
    {
        IL2CPP_ASSERT(info != 0 && "Debug::SetBreakpointDataAt be invoked on a null Il2CppDebugMethodInfo.");
        IL2CPP_ASSERT(location < info->code_size && "Debug::SetBreakpointDataAt invoked with an invalid location.");

        SequencePointRecord *found = NULL;

        for (int32_t i = 0; i < info->sequence_points_count; ++i)
        {
            SequencePointRecord *record = &info->sequence_points[i];

            if (location < record->start_offset)
                continue;

            if (record->end_offset < location)
                continue;

            found = record;
            break;
        }

        IL2CPP_ASSERT(found && "Debug::SetBreakpointDataAt invoked with an invalid location.");

        found->user_data = data;
    }

    void Debug::ClearBreakpointData(const Il2CppDebugMethodInfo* info)
    {
        IL2CPP_ASSERT(info != 0 && "Debug::ClearBreakpointData be invoked on a null Il2CppDebugMethodInfo.");

        for (int32_t i = 0; i < info->sequence_points_count; ++i)
        {
            SequencePointRecord *record = &info->sequence_points[i];

            record->user_data = NULL;
        }
    }

    void Debug::ClearBreakpointDataAt(const Il2CppDebugMethodInfo* info, uint64_t location)
    {
        IL2CPP_ASSERT(info != 0 && "Debug::ClearBreakpointDataAt be invoked on a null Il2CppDebugMethodInfo.");

        SequencePointRecord *found = NULL;

        for (int32_t i = 0; i < info->sequence_points_count; ++i)
        {
            SequencePointRecord *record = &info->sequence_points[i];

            if (location < record->start_offset)
                continue;

            if (record->end_offset < location)
                continue;

            found = record;
            break;
        }

        IL2CPP_ASSERT(found && "Debug::ClearBreakpointDataAt invoked with an invalid location.");

        found->user_data = NULL;
    }
} /* namespace vm */
} /* namespace il2cpp */

#endif
