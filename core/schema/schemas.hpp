#pragma once
#include <sdk/interfaces/schema_system.hpp>
#include <util/mem/process.hpp>
#include <map>

#define SCHEMA_FIELD(type, name, table_name, field_name) type name() { static auto offset = schemas::_::_offsets[table_name][field_name]; return process::read_memory<type>(u64(this) + offset); }
#define SCHEMA_FIELD_OFFSET(type, name, offset) type name() { return process::read_memory<type>(u64(this) + offset); }

namespace schemas {
	namespace _ {
		inline std::map<std::string, std::map<std::string, i64>> _offsets;
	}
	void init(cstrike::SchemaSystem* schema_system);

	__forceinline u64 get_offset(const std::string& table_name, const std::string& field_name) {
		return _::_offsets[table_name][field_name];
	}

}
