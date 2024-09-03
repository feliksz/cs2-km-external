#pragma once
#include <vector>
#include <sdk/entities/player_controller.hpp>
#include <sdk/entities/player_pawn.hpp>

namespace cstrike::entity_manager {

	struct CachedPlayer_t {
		i32 idx;
		PlayerController* controller;
		PlayerPawn* pawn;
		std::vector<V3> bones;
	};

	namespace _ {
		void run();

		std::vector<V3> get_bones(PlayerPawn* pawn);

		inline std::vector<CachedPlayer_t> _cached_players;
	}


	void init();

	__forceinline std::vector<CachedPlayer_t>& players() {
		return _::_cached_players;
	}

}