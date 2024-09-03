#include <core/overlay/overlay.hpp>
#include <util/mem/module.hpp>
#include <util/mem/process.hpp>
#include "entity_manager/entity_manager.hpp"
#include "cstrike.hpp"

using namespace cstrike::_;

constexpr static auto SIG_ENTITYLIST = "48 8B 0D ? ? ? ? 48 89 7C 24 ? 8B FA C1 EB";
constexpr static auto SIG_LOCAL_PLAYER_CONTROLLER = "48 8B 05 ? ? ? ? 48 85 C0 74 4F";
constexpr static auto SIG_VIEWMATRIX = "48 8D 0D ? ? ? ? 48 C1 E0 06";
constexpr static auto SIG_SCHEMASYSTEM = "48 8D 0D ? ? ? ? E9 ? ? ? ? CC CC CC CC 48 8D 0D ? ? ? ? E9 ? ? ? ? CC CC CC CC 48 83 EC 28";
constexpr static auto SIG_PLANTED_C4 = "48 8B 15 ? ? ? ? FF C0 48 8D 4C 24 40";

void cstrike::init() {
	auto client_dll = process::find_module("client.dll");
	auto schemasystem_dll = process::find_module("schemasystem.dll");


	_view_matrix_address = client_dll.find_pattern(SIG_VIEWMATRIX, true);
	if (!_view_matrix_address)
		THROW("couldn't get view matrix.");

	LOG("[cstrike] view_matrix -> 0x%llx", _view_matrix_address);


	entity_list = process::read_memory<EntityList*>(client_dll.find_pattern(SIG_ENTITYLIST, true));
	if (!entity_list)
		THROW("couldn't get entity list.");

	LOG("[cstrike] entity_list -> 0x%llx", (u64)entity_list);


	schema_system = (SchemaSystem*)schemasystem_dll.find_pattern(SIG_SCHEMASYSTEM, true);
	if (!schema_system)
		THROW("couldn't get schema system.");

	LOG("[cstrike] schema_system -> 0x%llx", (u64)schema_system);


	_local_player_controller_address = client_dll.find_pattern(SIG_LOCAL_PLAYER_CONTROLLER, true);
	if (!_local_player_controller_address)
		THROW("couldn't get local player controller.");

	LOG("[cstrike] local_player -> 0x%llx", _local_player_controller_address);


	LOG("[cstrike] initializing schema manager...");
	schemas::init(schema_system);

	LOG("[cstrike] initializing entity manager...");
	entity_manager::init();
}

void cstrike::update() {
	static auto  client_dll = process::find_module("client.dll");
	view = process::read_memory<ViewMatrix>(_view_matrix_address);

	local_player_controller = process::read_memory<PlayerController*>(_local_player_controller_address);
	local_player_pawn = entity_list->getEntity<PlayerPawn>(local_player_controller->getPawn());
}



bool cstrike::world_to_screen(const V3& in, V2i& out) {
	return view.world_to_screen(in, out);
}