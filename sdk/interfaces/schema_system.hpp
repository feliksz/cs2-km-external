#pragma once
#include <util/mem/process.hpp>
#include <common.hpp>

#include <sdk/classes/utl_ts_hash.hpp>
namespace cstrike {
	class SchemaClassFieldData {
	public:
		std::string get_name() const {
			return process::read_string(process::read_memory<>(u64(this)), 64);
		}

		u16 get_offset() const {
			return process::read_memory<u16>(u64(this) + 0x10);
		}
	};
	class SchemaClassInfo {
	public:
		std::vector<SchemaClassFieldData*> get_fields() const {
			std::vector<SchemaClassFieldData*> ret;
			for (auto i = 0; i < get_fields_count(); i++) {
				const auto field = (SchemaClassFieldData*)process::read_memory<>(u64(this) + 0x28) + (i * 0x20);
				if (!field)
					continue;

				ret.push_back(field);
			}
			return ret;
		}

		u32 get_fields_count() const {
			return process::read_memory<u16>(reinterpret_cast<u64>(this) + 0x1C);
		}
		std::string get_module_name() const {
			return process::read_string(process::read_memory<>(u64(this) + 0x8), 64);
		}
	};

	class SchemaTypeDeclaredClass {
	public:
	};
	class SchemaSystemTypeScope {
	public:
		std::vector<SchemaClassInfo*> get_classes() {
			const auto classes = process::read_memory<sdk::UtlTsHash<SchemaTypeDeclaredClass*>>(u64(this) + 0x5B8);
			
			std::vector<SchemaClassInfo*> ret;
			for (auto& e : classes.elements()) {
				ret.push_back((SchemaClassInfo*)e);
			}
			return ret;
		}
		std::string get_module_name() const {
			return process::read_string(reinterpret_cast<u64>(this) + 0x8, 256);
		}
	};
	class SchemaSystem {
	public:
		u32 get_count() const {
			return process::read_memory<u32>(reinterpret_cast<u64>(this) + 0x190);
		}

		std::vector<SchemaSystemTypeScope*> get_type_scopes() const {
			const auto data = process::read_memory<u64>(reinterpret_cast<u64>(this) + 0x198);
			if (!data)
				return {};

			std::vector<SchemaSystemTypeScope*> type_scopes;
			type_scopes.resize(get_count());

			process::read_memory(data, type_scopes.data(), type_scopes.size() * sizeof(u64));
			return type_scopes;
		}
	};
}