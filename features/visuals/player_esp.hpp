#pragma once
#include <sdk/entities/player_pawn.hpp>
#include <common.hpp>

namespace features::visuals::player_esp {
	void draw();

	V4i get_bounding_box(cstrike::PlayerPawn* pawn, const V3& head_bone_pos);
}