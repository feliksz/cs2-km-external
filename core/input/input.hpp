#pragma once
#include <array>
#include <common.hpp>

namespace input {
	namespace _ {
		inline HWND _hwnd;

		inline V2i _mouse_pos{ };
		inline std::array<bool, 256> _keys{}, _prev_keys{};
	}
	void init(HWND hwnd);

	void update();

	V2i get_mouse_pos();
	V2i get_raw_mouse_pos();

	bool is_mouse_in_region(const V2i& pos, const V2i& size);
	bool is_key_pressed(u32 key);
	bool is_key_down(u32 key);
}