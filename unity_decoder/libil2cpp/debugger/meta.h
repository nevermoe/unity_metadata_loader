#pragma once

#include <set>
#include <vector>
#include <string>

#include "common.h"
#include "data-buffer.h"

namespace il2cpp
{
namespace debugger
{

template<typename T>
struct Property;

#define DEFINE_SIMPLE_PROP_STD(Type, Writer, Reader) \
	template<> \
	struct Property<Type> \
	{ \
		inline const Type &operator()() const { return _storage; } \
		inline void operator()(const Type &v) { _storage = v; } \
		inline void WriteTo(Buffer &out) const { out. Writer (_storage); } \
		inline Type ReadFrom(Buffer &in) { return _storage = in. Reader (); } \
		inline operator Type() { return _storage; } \
	private: \
		Type _storage; \
	}

#define DEFINE_SIMPLE_PROP_INIT(Type, Writer, Reader) \
	template<> \
	struct Property<Type> \
	{ \
		Property() : _storage( 0 ) {} \
		inline Type operator()() const { return _storage; } \
		inline void operator()(Type v) { _storage = v; } \
		inline void WriteTo(Buffer &out) const { out. Writer (_storage); } \
		inline void ReadFrom(Buffer &in) { _storage = in. Reader (); } \
		inline operator Type() { return _storage; } \
	private: \
		Type _storage; \
	}

#define DEFINE_LIST_PROP(Type, Writer, Reader) \
	template<> \
	struct Property<std::vector<Type> > \
	{ \
		Property() { } \
		inline std::vector<Type> &operator()() { return _storage; } \
		inline const std::vector<Type> &operator()() const { return _storage; } \
		inline operator std::vector<Type> &() { return _storage; } \
		inline void WriteTo(Buffer &out) const { \
			out.WriteInt(_storage.size()); \
			std::vector<Type>::const_iterator it = _storage.begin(); \
			while(it != _storage.end()) { \
				out. Writer (*it); \
				++it; \
			} \
		} \
		inline void ReadFrom(Buffer &in) { \
			const int32_t len = in.ReadInt(); \
			_storage.resize(len); \
			for(int32_t i = 0; i < len; ++i) { \
				_storage[i] = in. Reader (); \
			} \
		} \
	private: \
		std::vector<Type> _storage; \
	};

#define DEFINE_SET_PROP(Type, Writer, Reader) \
	template<> \
	struct Property<std::set<Type> > \
	{ \
		Property() { } \
		inline std::set<Type> &operator()() { return _storage; } \
		inline operator std::set<Type> &() { return _storage; } \
		inline void WriteTo(Buffer &out) const { \
			out.WriteInt(_storage.size()); \
			std::set<Type>::const_iterator it = _storage.begin(); \
			while(it != _storage.end()) { \
				out. Writer (*it); \
				++it; \
			} \
		} \
		inline void ReadFrom(Buffer &in) { \
			const int32_t len = in.ReadInt(); \
			for(int32_t i = 0; i < len; ++i) { \
				_storage.insert(in. Reader ()); \
			} \
		} \
	private: \
		std::set<Type> _storage; \
	};

DEFINE_SIMPLE_PROP_STD(std::string, WriteString, ReadString);
DEFINE_SIMPLE_PROP_INIT(bool, WriteBool, ReadBool);
DEFINE_SIMPLE_PROP_INIT(int32_t, WriteInt, ReadInt);
DEFINE_SIMPLE_PROP_INIT(uint8_t, WriteByte, ReadByte);
DEFINE_SIMPLE_PROP_INIT(uint64_t, WriteLong, ReadLong);
DEFINE_SIMPLE_PROP_INIT(Il2CppAssembly*, WriteAssemblyId, ReadAssembly);
DEFINE_SIMPLE_PROP_INIT(Il2CppClass*, WriteTypeId, ReadType);
DEFINE_SIMPLE_PROP_INIT(const MethodInfo*, WriteMethodId, ReadMethod);
DEFINE_SIMPLE_PROP_INIT(Il2CppImage*, WriteModuleId, ReadModule);
DEFINE_SIMPLE_PROP_INIT(Il2CppDomain*, WriteDomainId, ReadDomain);
DEFINE_SIMPLE_PROP_INIT(Il2CppObject*, WriteObjectId, ReadObject);
DEFINE_SIMPLE_PROP_INIT(Il2CppThread*, WriteThreadId, ReadThread);
DEFINE_SIMPLE_PROP_INIT(FieldInfo*, WriteFieldId, ReadField);
DEFINE_SIMPLE_PROP_INIT(const Il2CppImage*, WriteModuleId, ReadModule);

DEFINE_LIST_PROP(std::string, WriteString, ReadString);
DEFINE_LIST_PROP(bool, WriteBool, ReadBool);
DEFINE_LIST_PROP(int32_t, WriteInt, ReadInt);
DEFINE_LIST_PROP(uint8_t, WriteByte, ReadByte);
DEFINE_LIST_PROP(uint64_t, WriteLong, ReadLong);
DEFINE_LIST_PROP(Il2CppAssembly*, WriteAssemblyId, ReadAssembly);
DEFINE_LIST_PROP(Il2CppClass*, WriteTypeId, ReadType);
DEFINE_LIST_PROP(const MethodInfo*, WriteMethodId, ReadMethod);
DEFINE_LIST_PROP(Il2CppImage*, WriteModuleId, ReadModule);
DEFINE_LIST_PROP(Il2CppDomain*, WriteDomainId, ReadDomain);
DEFINE_LIST_PROP(Il2CppObject*, WriteObjectId, ReadObject);
DEFINE_LIST_PROP(Il2CppThread*, WriteThreadId, ReadThread);
DEFINE_LIST_PROP(FieldInfo*, WriteFieldId, ReadField);

DEFINE_SET_PROP(std::string, WriteString, ReadString);
DEFINE_SET_PROP(bool, WriteBool, ReadBool);
DEFINE_SET_PROP(int32_t, WriteInt, ReadInt);
DEFINE_SET_PROP(uint8_t, WriteByte, ReadByte);
DEFINE_SET_PROP(uint64_t, WriteLong, ReadLong);
DEFINE_SET_PROP(Il2CppAssembly*, WriteAssemblyId, ReadAssembly);
DEFINE_SET_PROP(Il2CppClass*, WriteTypeId, ReadType);
DEFINE_SET_PROP(const MethodInfo*, WriteMethodId, ReadMethod);
DEFINE_SET_PROP(Il2CppImage*, WriteModuleId, ReadModule);
DEFINE_SET_PROP(Il2CppDomain*, WriteDomainId, ReadDomain);
DEFINE_SET_PROP(Il2CppObject*, WriteObjectId, ReadObject);
DEFINE_SET_PROP(Il2CppThread*, WriteThreadId, ReadThread);
DEFINE_SET_PROP(FieldInfo*, WriteFieldId, ReadField);

} /* namespace debugger */
} /* namespace il2cpp */
