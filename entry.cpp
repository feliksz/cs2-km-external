#include <core/input/input.hpp>
#include <core/overlay/overlay.hpp>
#include <core/cstrike.hpp>
#include <util/mem/driver.hpp>
#include <util/mem/process.hpp>
#include <util/render/render.hpp>
#include <util/winapi/winapi.hpp>

#include <features/visuals/player_esp.hpp>
#include <features/visuals/rank_reveal.hpp>
#include <features/visuals/spectator_list.hpp>

int main() {

	try {
		LOG("[ts] initializing driver...");

		driver::init("dupa");

		LOG("[ts] waiting for cs2.exe...");

		while (!process::find("cs2.exe"))
			std::this_thread::sleep_for(std::chrono::milliseconds(250));

		const auto cs2_window = FindWindowA(0, "Counter-Strike 2");
		system("cls");

		LOG("[ts] initializing process...");
		process::init();

		LOG("[ts] initializing game...");
		cstrike::init();

		LOG("[ts] initializing overlay...");
		overlay::init(cs2_window, "tsvip v2");

		LOG("[ts] initializing input...");
		input::init(cs2_window);

		LOG("[ts] initializing renderer...");
		render::init(overlay::get_hwnd(), 240);

		LOG("[ts] initialized, hf!");

		_perf_timers["render"] = _perf_timers["input"] = PerfomanceTimer();

		std::thread t_input([&]() {
			while (overlay::_::_is_running) {
				input::update();

				if (input::is_key_pressed(VK_DELETE)) {
					overlay::_::_is_running = false;
				}

				features::misc::rank_reveal::update_list();

				std::this_thread::sleep_for(std::chrono::milliseconds(6));
			}
			});
		t_input.detach();

		while (overlay::run()) {
			auto perf_timer = &_perf_timers["render"];

			cstrike::update();

			render::begin(); {
				constexpr static auto watermark_text = "twojastara-vip (" __DATE__ ")";
				render::text({ 11, 11 }, Color(255, 0, 0, 255), watermark_text);
				render::text({ 10, 10 }, Color(255, 255, 0, 255), watermark_text);


				features::visuals::player_esp::draw();
				features::visuals::spectator_list::draw();
				features::misc::rank_reveal::draw();

				int i = 0;
				for (auto& pf : _perf_timers) {
					const auto pos = V2i{ 10, 24 + i };
					const auto text = std::format("{}: \t{}ms\n\t\t{}fps", pf.first, pf.second.get_last_frametime(), pf.second.get_fps());
					render::text({ pos.x + 1, pos.y + 1 }, Color(0, 0, 0, 255), text);
					render::text(pos, Color(255, 255, 255, 255), text);

					i += 28;
				}
			}
			render::end();

			_perf_timers["render"].tick();
		}

		while (overlay::_::_is_running) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		overlay::release();
	} catch (const std::runtime_error& err) {
		LOG("%s", err.what());
		MessageBoxA(0, err.what(), "twojastara-vip", MB_OK);
	}
	std::cin.get();

}