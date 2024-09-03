#pragma once
#include <util/winapi/winapi.hpp>

namespace overlay {
	namespace _ {
		inline HWND _hwnd, _target_hwnd;
		inline WNDCLASSEX _wc{};

		inline V2i _screen_size;

		inline bool _is_running = false;
	}

	void init(HWND target, const std::string& title);
	void release();

	bool run();

	__forceinline auto get_hwnd() {
		return _::_hwnd;
	}

	V2i get_screen_size();
	i32 get_screen_refresh_rate();
}