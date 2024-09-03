#pragma once
#include <util/mem/process.hpp>
#include <sdk/entities/base_entity.hpp>

namespace cstrike {
	class EntityList {
	public:
		BaseEntity* getBaseEntity(i32 idx) {
			auto entry = process::read_memory<>(u64(this) + (8 * ((idx & 0x7FFF) >> 9) + 16));
			if (!entry)
				return nullptr;

			return process::read_memory<BaseEntity*>(entry + (120 * (idx & 0x1FF)));
		}

		template <typename T = BaseEntity> T* getEntity(i32 idx) {
			return (T*)getBaseEntity(idx);
		}
	};
}