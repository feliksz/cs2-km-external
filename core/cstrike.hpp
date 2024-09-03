#pragma once
#include <common.hpp>
#include <sdk/entities/player_controller.hpp>
#include <sdk/interfaces/entity_list.hpp>
#include <sdk/interfaces/schema_system.hpp>
#include <sdk/interfaces/view_matrix.hpp>
#include "entity_manager/entity_manager.hpp"
#include "schema/schemas.hpp"

namespace cstrike {
	namespace _ {
		inline addr_t _view_matrix_address;
		inline addr_t _local_player_controller_address;
	}

	inline EntityList* entity_list;
	inline SchemaSystem* schema_system;
	inline ViewMatrix view;

	inline PlayerController* local_player_controller;
	inline PlayerPawn* local_player_pawn;

	void init();
	void update();

	bool world_to_screen(const V3& in, V2i& out);
}