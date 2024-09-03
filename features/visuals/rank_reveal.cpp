#include <core/input/input.hpp>
#include <core/cstrike.hpp>
#include <util/render/render.hpp>
#include "rank_reveal.hpp"

std::array<std::string, 19> RANK_NAMES = {
	"n/a",
	"silver 1",
	"silver 2",
	"silver 3",
	"silver 4",
	"silver 5",
	"silver 6",
	"gold 1",
	"gold 2",
	"gold 3",
	"gold 4",
	"master guardian 1",
	"master guardian 2",
	"master guardian 3",
	"distinguished master guardian",
	"legendary eagle",
	"legendary eagle master",
	"supreme master first class",
	"the global elite"
};

struct RankRevealListEntry_t {
	std::string name;
	std::string rank_name;
	i32 wins;
};

static std::vector<RankRevealListEntry_t> entries{};

void features::misc::rank_reveal::draw() {
	if (input::is_key_down(VK_TAB)) {
		auto y_offset = 0;
		for (auto& entry : entries) {
			auto entry_text = std::format("{} {: >11} {}", entry.name, entry.rank_name, entry.wins);
			render::text({ 11, 121 + y_offset }, Color(0, 0, 0, 180), entry_text);
			render::text({ 10, 120 + y_offset }, Color(255, 255, 255, 255), entry_text);
			y_offset += 16;
		}
	}
}

void features::misc::rank_reveal::update_list() {
	std::vector<RankRevealListEntry_t> new_entries;

	for (auto i = 1; i < 64; i++) {
		auto entity = cstrike::entity_list->getEntity<cstrike::PlayerController>(i);
		if (!entity)
			continue;

		auto pawn = cstrike::entity_list->getEntity<cstrike::PlayerPawn>(entity->getPawn());
		if (!pawn)
			continue;

		const auto comp_rank = entity->getCompetitiveRanking();
		new_entries.push_back({ entity->getSanitizedName(), comp_rank >= 0 && comp_rank < 18 ? RANK_NAMES[comp_rank] : "n/a", entity->getCompetitiveWins() });
	}

	entries = new_entries;
}
