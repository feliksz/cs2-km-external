#include "input.hpp"

using namespace input::_;

void input::init(HWND hwnd) {
	_hwnd = hwnd;
}

void input::update() {
	static auto kurwa = &_perf_timers["input"];
	auto mp = POINT{};
	GetCursorPos(&mp);
	ScreenToClient(_hwnd, &mp);

	_mouse_pos = V2i{ mp.x, mp.y };

	memcpy_s(_prev_keys.data(), sizeof(_prev_keys), _keys.data(), sizeof(_keys));

	for (auto i = 0; i < 256; i++) {
		_keys[i] = GetAsyncKeyState(i);
	}

	kurwa->tick();
}

V2i input::get_mouse_pos() {
	return _mouse_pos;
}

V2i input::get_raw_mouse_pos() {
	return V2i();
}

bool input::is_mouse_in_region(const V2i& pos, const V2i& size) {
	return _mouse_pos.x >= pos.x && _mouse_pos.y >= pos.y && _mouse_pos.x <= pos.x + size.x && _mouse_pos.y <= pos.y + size.y;
}

bool input::is_key_pressed(u32 key) {
	return !_prev_keys.at(key) && _keys.at(key);
}

bool input::is_key_down(u32 key) {
	return _prev_keys.at(key) && _keys.at(key);
}
