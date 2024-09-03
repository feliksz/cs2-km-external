#pragma once
#include "base_entity.hpp"

namespace cstrike {
	struct BoneData_t {
		V3 position;
		char __pad[0x14];
	};

	using BoneArray_t = std::array<BoneData_t, 30>;

	class GameSceneNode {
	public:
		SCHEMA_FIELD_OFFSET(u64, getBoneArray, 0x1E0);

		V3 getBonePosition(i32 bone_idx) {
			BoneData_t bone_data[30]{};
			process::read_memory(getBoneArray(), bone_data, 30 * sizeof(BoneData_t));

			return bone_data[bone_idx].position;
		}
	};

	class PlayerObserverService : public BaseEntity {
	public:
		SCHEMA_FIELD(u8, getObserverMode, "CPlayer_ObserverServices", "m_iObserverMode");
		SCHEMA_FIELD(u32, getObserverTarget, "CPlayer_ObserverServices", "m_hObserverTarget");
	};
	class PlayerPawn : public BaseEntity {
	public:
		SCHEMA_FIELD(u64, getGameSceneNode, "C_BaseEntity", "m_pGameSceneNode");
		//u64 getGameSceneNode() {
		//	return process::read_memory<u64>(u64(this) + 0x310);
		//}
		
		BoneArray_t getBoneArray() {
			const auto bone_array = process::read_memory<u64>(getGameSceneNode() + 0x1E0);
			auto bone_data = BoneArray_t{};

			process::read_memory(bone_array, bone_data.data(), 30 * sizeof(BoneData_t));
			return std::move(bone_data);
		}

		V3 getBonePosition(i32 bone_idx) {
			return getBoneArray()[bone_idx].position;
			const auto bone_array = process::read_memory<u64>(getGameSceneNode() + 0x1E0);
			if (!bone_array)
				return V3{0, 0, 0};

			return process::read_memory<V3>(bone_array + bone_idx * 32);
		}

		SCHEMA_FIELD(u32, getController, "C_BasePlayerPawn", "m_hController");
		SCHEMA_FIELD(V3, getOrigin, "C_BasePlayerPawn", "m_vOldOrigin");

		SCHEMA_FIELD(u32, getTargetIndex, "C_CSPlayerPawnBase", "m_iIDEntIndex");
		SCHEMA_FIELD(PlayerObserverService*, getObserverService, "C_BasePlayerPawn", "m_pObserverServices");
	};
}