#include "../cstrike.hpp"
#include "entity_manager.hpp"

void cstrike::entity_manager::init() {
	_perf_timers["entity manager"] = PerfomanceTimer();

	std::thread entity_manager_thread([&]() {
		auto perf_timer = &_perf_timers["entity manager"];
		while (true) {
			_::run();
			perf_timer->tick();
		}
		});

	entity_manager_thread.detach();
}

void cstrike::entity_manager::_::run() {
	std::vector<CachedPlayer_t> players;

	auto b = local_player_controller->getTeamNum();
	for (auto i = 0; i < 64; i++) {
		auto entity = entity_list->getBaseEntity(i);
		if (!entity)
			continue;

		auto player = (PlayerController*)entity;
		auto pawn_id = player->getPawn();
		if (!pawn_id)
			continue;

		auto pawn = entity_list->getEntity<PlayerPawn>(pawn_id);
		if (!pawn)
			continue;

		if (pawn->getHealth() <= 0)
			continue;

		if (player->getTeamNum() == local_player_controller->getTeamNum())
			continue;

		players.push_back(CachedPlayer_t{ i, player, pawn, _::get_bones(pawn)});
	}

	_cached_players = players;

	std::this_thread::sleep_for(std::chrono::milliseconds(6));
}

std::vector<V3> cstrike::entity_manager::_::get_bones(PlayerPawn* pawn) {
	std::vector<V3> ret;
	for (auto i = 0u; i < 30; i++) {
		ret.push_back(pawn->getBoneArray()[i].position);
	}
	return ret;
}
