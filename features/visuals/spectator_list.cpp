#include <core/cstrike.hpp>
#include <util/render/render.hpp>
#include "spectator_list.hpp"

struct SpectatorInfo_t {
	cstrike::PlayerPawn* target_pawn;
	cstrike::PlayerController* observer_controller, *target_controller;
	u8 mode;
};

void features::visuals::spectator_list::draw() {
	const auto local_player_observer_service = cstrike::local_player_pawn->getObserverService();
	const auto local_player_observer_target = local_player_observer_service->getObserverTarget();

	cstrike::PlayerPawn* observer_target;
	if (local_player_observer_target) {
		observer_target = cstrike::entity_list->getEntity<cstrike::PlayerPawn>(local_player_observer_target);
	} else {
		observer_target = cstrike::local_player_pawn;
	}

	std::vector<SpectatorInfo_t> spectators;

	for (auto i = 0; i < 64; i++) {
		auto entity = cstrike::entity_list->getBaseEntity(i);
		if (!entity)
			continue;

		auto player = (cstrike::PlayerController*)entity;
		auto pawn_id = player->getPawn();
		if (!pawn_id)
			continue;

		auto pawn = cstrike::entity_list->getEntity<cstrike::PlayerPawn>(pawn_id);
		if (!pawn)
			continue;

		auto observer_service = pawn->getObserverService();
		if (!observer_service)
			continue;

		auto target_pawn = cstrike::entity_list->getEntity<cstrike::PlayerPawn>(observer_service->getObserverTarget());
		if (!target_pawn)
			continue;

		auto target_controller = cstrike::entity_list->getEntity<cstrike::PlayerController>(target_pawn->getController());
		if (!target_controller)
			continue;

		spectators.push_back(SpectatorInfo_t{ target_pawn, player, target_controller, observer_service->getObserverMode() });
	}

	render::text({ 11, 301 }, Color(0, 0, 0, 200), std::format("Spectators:"));
	render::text({ 10, 300 }, Color(255, 255, 255, 255), std::format("Spectators:"));

	auto y_pos = 0;
	for (auto& spec : spectators) {
		if (spec.target_pawn != observer_target)
			continue;

		render::text({ 11, 321 + y_pos }, Color(0, 0, 0, 200), spec.observer_controller->getSanitizedName());
		render::text({ 10, 320 + y_pos }, Color(255, 255, 255, 255), spec.observer_controller->getSanitizedName());
		y_pos += 16;	
	}
}
