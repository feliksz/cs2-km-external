#pragma once
#include <string>
#include <core/schema/schemas.hpp>
#include "base_entity.hpp"

namespace cstrike {
	class PlayerController : public BaseEntity {
	public:
		//SCHEMA_FIELD(u64, getPawn, "CBasePlayerController", "m_hPawn");

		u64 getPawn() {
			return process::read_memory<u64>(u64(this) + 0x604);
		}

		SCHEMA_FIELD(i32, getHealth, "CCSPlayerController", "m_iPawnHealth");
		SCHEMA_FIELD(i32, getCompetitiveRanking, "CCSPlayerController", "m_iCompetitiveRanking");
		SCHEMA_FIELD(i32, getCompetitiveWins, "CCSPlayerController", "m_iCompetitiveWins");

		std::string getSanitizedName() {
			static auto offset = schemas::get_offset("CCSPlayerController", "m_sSanitizedPlayerName");

			char buffer[256]{};
			process::read_memory(process::read_memory<>(u64(this) + offset), buffer, 256);

			return std::string(buffer);
		}

	};
}