#include <core/cstrike.hpp>
#include <util/render/render.hpp>
#include "player_esp.hpp"

void draw_player_name(cstrike::PlayerController* controller, const V4i& bbox) {
    const auto player_name = controller->getSanitizedName();
    const auto text_size = render::get_text_size(player_name);

    render::text({ bbox.x + (bbox.w / 2) - (text_size.x / 2) + 1, bbox.y - text_size.y - 3 }, Color(0, 0, 0, 255), player_name);
    render::text({ bbox.x + (bbox.w / 2) - (text_size.x / 2), bbox.y - text_size.y - 4 }, Color(255, 255, 255, 255), player_name);
}

void draw_player_healthbar(cstrike::PlayerPawn* pawn, const V4i& bbox) {
    const auto health_percentage = f32(pawn->getHealth()) / 100.f;
    const auto healthbar_height = i32(bbox.h * health_percentage);

    render::rect({ bbox.x - 8, bbox.y - 1 }, { 5, bbox.h + 2 }, Color(0, 0, 0, 90), 5.f);
    render::fill({ bbox.x - 7, bbox.y }, { 3, bbox.h }, Color(0, 0, 0, 200), 5.f);
    render::fill({ bbox.x - 7, bbox.y + bbox.h - healthbar_height }, { 3, healthbar_height }, Color(0, 255, 0, 255), 5.f);
}

void draw_player_skeleton(cstrike::PlayerPawn* pawn, const std::vector<V3>& bone_array) {
    std::vector<V2i> bone_screen_pos;
    bone_screen_pos.resize(30);

    const auto b_array = pawn->getBoneArray();
	for (auto i = 0; i < 29; i++) {
        V2i screen_pos;
        if (!cstrike::view.world_to_screen(b_array[i].position, screen_pos))
            continue;

        bone_screen_pos[i] = screen_pos;
	}

    std::vector<std::pair<V2i, V2i>> points = {
    {bone_screen_pos[6], bone_screen_pos[5] },
    {bone_screen_pos[5], bone_screen_pos[8] },
    {bone_screen_pos[5], bone_screen_pos[13] },
    {bone_screen_pos[13], bone_screen_pos[14]},
    {bone_screen_pos[8], bone_screen_pos[9]},
    {bone_screen_pos[14], bone_screen_pos[16]},
    {bone_screen_pos[9], bone_screen_pos[11]},
    {bone_screen_pos[5], bone_screen_pos[0]},
    {bone_screen_pos[0], bone_screen_pos[23]},
    {bone_screen_pos[0], bone_screen_pos[26]},
    {bone_screen_pos[23], bone_screen_pos[24]},
    {bone_screen_pos[26], bone_screen_pos[27]}
    };

    for (auto& ps : points) {
        render::line(ps.first, ps.second, Color(255, 255, 255, 180));
    }
}

void features::visuals::player_esp::draw() {
	for (auto& player : cstrike::entity_manager::players()) {
        const auto bbox = get_bounding_box(player.pawn, player.bones[6]);
        if (!bbox)
            continue;

        draw_player_name(player.controller, bbox);
        draw_player_healthbar(player.pawn, bbox);
        draw_player_skeleton(player.pawn, player.bones);
	}

}

V4i features::visuals::player_esp::get_bounding_box(cstrike::PlayerPawn* pawn, const V3& head_bone_pos) {
    const auto pos = pawn->getOrigin();
    auto head_pos = pawn->getBonePosition(6);

    V2i screen_pos{}, head_screen_pos{};
    if (!cstrike::view.world_to_screen(pos, screen_pos))
        return V4i{};
    
    if (!cstrike::view.world_to_screen(head_pos, head_screen_pos)) {
        head_pos = pos;
        head_pos.z += 3.5f;
        
        if (!cstrike::view.world_to_screen(head_pos, head_screen_pos))
            return V4i{};
    }
    V4i bbox;
    bbox.h = screen_pos.y - head_screen_pos.y;
    bbox.w = i32(bbox.h / 2.2f);
    
    bbox.x = screen_pos.x - (bbox.w / 2);
    bbox.y = screen_pos.y - bbox.h;
    return bbox;
}
