#pragma once
#include <core/schema/schemas.hpp>

namespace cstrike {
	class BaseEntity {
	public:
		SCHEMA_FIELD(i32, getTeamNum, "C_BaseEntity", "m_iTeamNum");
		SCHEMA_FIELD(i32, getHealth, "C_BaseEntity", "m_iHealth");
	};
}