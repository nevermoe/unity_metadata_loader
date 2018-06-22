#if IL2CPP_DEBUGGER_ENABLED

#pragma once

#include "../../il2cpp-api.h"
#include "../../blob.h"
#include "../../tabledefs.h"
#include "../../class-internals.h"
#include "../../object-internals.h"
#include "../../vm/Class.h"

#include "../agent.h"
#include "../variant.h"
#include "../protocol/commands/type.h"
#include "../protocol/commands/internal-error.h"

using il2cpp::vm::Class;

namespace il2cpp
{
namespace debugger
{
    const Reply *Agent::Process(const TypeGetInfoCommand *command)
    {
        TypeGetInfoCommand::Reply *get_info_reply = command->reply();

        Il2CppClass *klass = command->type();
        const Il2CppImage *image = il2cpp_class_get_image(klass);
        const Il2CppType *type = il2cpp_class_get_type(klass);
        char *fullname = il2cpp_type_get_name(type);

        get_info_reply->ns(il2cpp_class_get_namespace(klass));
        get_info_reply->name(il2cpp_class_get_name(klass));
        get_info_reply->fullname(fullname);
        get_info_reply->assembly(const_cast<Il2CppAssembly*>(il2cpp_image_get_assembly(image)));
        get_info_reply->module(const_cast<Il2CppImage*>(image));
        get_info_reply->parent_type(il2cpp_class_get_parent(klass));
        if (klass->rank || klass->byval_arg->type == IL2CPP_TYPE_PTR)
            get_info_reply->underlying_type(il2cpp_class_get_element_class(klass)); // TODO: fix
        else
            get_info_reply->underlying_type(0);
        get_info_reply->type_token(il2cpp_type_get_type(type));
        get_info_reply->rank(klass->rank);
        get_info_reply->flags(il2cpp_class_get_flags(klass));
        // get_info_reply->nested_types(); // TODO: fix

        uint8_t by_val_flags = 0;
        if (type->type == IL2CPP_TYPE_PTR)
            by_val_flags |= (1 << 1);
        if (!type->byref && (((type->type >= IL2CPP_TYPE_BOOLEAN) && (type->type <= IL2CPP_TYPE_R8)) || (type->type == IL2CPP_TYPE_I) || (type->type == IL2CPP_TYPE_U)))
            by_val_flags |= (1 << 2);
        if (type->type == IL2CPP_TYPE_VALUETYPE)
            by_val_flags |= (1 << 3);
        if (klass->enumtype)
            by_val_flags |= (1 << 4);
        if (Class::IsGeneric(klass))
            by_val_flags |= (1 << 5);
        if (Class::IsGeneric(klass) || klass->generic_class)
            by_val_flags |= (1 << 6);

        get_info_reply->by_val_flags(by_val_flags);

        delete[] fullname;

        LOG("warning: `TypeGetInfoReply` is not properly set up yet in `Agent::Process(const TypeGetInfoCommand *command)`.");

        return get_info_reply;
    }

    const Reply *Agent::Process(const TypeGetSourceFilesCommand *command)
    {
        TypeGetSourceFilesCommand::Reply *get_source_files_reply = command->reply();

        const Il2CppClass *type = command->type();
        const Il2CppDebugTypeInfo *debug_info = il2cpp_debug_get_class_info(type);

        if (!debug_info)
            return get_source_files_reply;

        const Il2CppDebugDocument *document = il2cpp_debug_class_get_document(debug_info);
        if (!document)
            return get_source_files_reply;

        std::string basename = il2cpp_debug_document_get_filename(document);

        // TODO: we need to iterate through all the methods and register the documents where the method is defined
        LOG("warning: `TypeGetSourceFiles` still doesn't deal with partial type definitions. Only one source file is always returned.");

        get_source_files_reply->source_files().insert(basename);

        return get_source_files_reply;
    }

    const Reply *Agent::Process(const TypeGetSourceFiles2Command *command)
    {
        TypeGetSourceFiles2Command::Reply *get_source_files_reply = command->reply();

        const Il2CppClass *type = command->type();
        const Il2CppDebugTypeInfo *debug_info = il2cpp_debug_get_class_info(type);

        if (!debug_info)
            return get_source_files_reply;

        const Il2CppDebugDocument *document = il2cpp_debug_class_get_document(debug_info);
        if (!document)
            return get_source_files_reply;

        std::string basename = il2cpp_debug_document_get_filename(document);
        std::string path = il2cpp_debug_document_get_directory(document);

        // TODO: we need to iterate through all the methods and register the documents where the method is defined
        LOG("warning: `TypeGetSourceFiles2Command` still doesn't deal with partial type definitions. Only one source file is always returned.");

        IL2CPP_ASSERT((path.size() == 0 || (path.back() == '/' || path.back() == '\\')) && "Il2CppDebugDocument::directory is expected to end with a platform-specific trailing slash.");

        get_source_files_reply->source_files().insert(path + basename);

        return get_source_files_reply;
    }

    const Reply *Agent::Process(const TypeGetPropertiesCommand *command)
    {
        TypeGetPropertiesCommand::Reply *get_properties_reply = command->reply();

        void* iterator = NULL;
        const PropertyInfo *prop = NULL;

        while (prop = il2cpp_class_get_properties(command->type(), &iterator))
        {
            get_properties_reply->properties.push_back(prop);
        }

        return get_properties_reply;
    }

    const Reply *Agent::Process(const TypeGetPropertyCAttrsCommand *command)
    {
        TypeGetPropertyCAttrsCommand::Reply *get_cattrs_reply = command->reply();

        get_cattrs_reply->fake_count(0);

        LOG("warning: `TypeGetPropertyCAttrsCommand` needs to be implemented properly. For now it only returns 0 attributes!");

        return get_cattrs_reply;
    }

    const Reply *Agent::Process(const TypeGetFieldCAttrsCommand *command)
    {
        TypeGetFieldCAttrsCommand::Reply *get_cattrs_reply = command->reply();

        get_cattrs_reply->fake_count(0);

        LOG("warning: `TypeGetFieldCAttrsCommand` needs to be implemented properly. For now it only returns 0 attributes!");

        return get_cattrs_reply;
    }

    const Reply *Agent::Process(const TypeIsAssignableFromCommand *command)
    {
        LOG("warning: `TypeIsAssignableFromCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }

    const Reply *Agent::Process(const TypeGetCAttrsCommand *command)
    {
        TypeGetCAttrsCommand::Reply *get_cattrs_reply = command->reply();

        get_cattrs_reply->fake_count(0);

        LOG("warning: `TypeGetCAttrsCommand` needs to be implemented properly. For now it only returns 0 attributes!");

        return get_cattrs_reply;
    }

    const Reply *Agent::Process(const TypeGetValuesCommand *command)
    {
        TypeGetValuesCommand::Reply *get_values_reply = command->reply();

        std::vector<FieldInfo*>::const_iterator it = command->fields().begin();

        while (it != command->fields().end())
        {
            FieldInfo *field = *it;

            if (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC))
            {
                LOG("error: TypeGetValuesCommand is expected to receive only static field IDs");
                get_values_reply->error_code(kErrorCodeInvalidFieldId);
                break;
            }

            /*if (il2cpp_class_field_is_special_static (field))
            {
                LOG("error: TypeGetValuesCommand is expected to receive only static field IDs");
                get_values_reply->error_code(kErrorCodeInvalidFieldId);
                break;
            }*/

            bool found = false;

            for (Il2CppClass *t = command->type(); t != 0; t = t->parent)
            {
                if (t == field->parent)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                LOG("error: TypeGetValuesCommand is expected to receive valid field IDs");
                get_values_reply->error_code(kErrorCodeInvalidFieldId);
                break;
            }

            get_values_reply->values().push_back(
                Variant(
                    field->type,
                    ((char*)field->parent->static_fields) + field->offset));

            ++it;
        }

        return get_values_reply;
    }

    const Reply *Agent::Process(const TypeGetObjectCommand *command)
    {
        TypeGetObjectCommand::Reply *get_object_reply = command->reply();

        get_object_reply->object(
            il2cpp_type_get_object(command->type()->byval_arg));

        return get_object_reply;
    }

    const Reply *Agent::Process(const TypeGetFieldsCommand *command)
    {
        TypeGetFieldsCommand::Reply *get_fields_reply = command->reply();

        void* iterator = NULL;
        FieldInfo *field = NULL;

        while (field = il2cpp_class_get_fields(command->type(), &iterator))
        {
            get_fields_reply->fields.push_back(field);
        }

        return get_fields_reply;
    }

    const Reply *Agent::Process(const TypeSetValuesCommand *command)
    {
        LOG("warning: `TypeSetValuesCommand` not implemented. Returning a `NotImplemented` reply!");

        IL2CPP_ASSERT(0);

        return new InternalErrorNotImplementedReply(command);
    }

    const Reply *Agent::Process(const TypeGetMethodsCommand *command)
    {
        TypeGetMethodsCommand::Reply *get_methods_reply = command->reply();

        void* iterator = NULL;
        const MethodInfo *method = NULL;

        while (method = il2cpp_class_get_methods(command->type(), &iterator))
        {
            get_methods_reply->methods().push_back(method);
        }

        return get_methods_reply;
    }
}
}

#endif
