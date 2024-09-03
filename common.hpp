#pragma once
#include <chrono>
#include <cstdint>
#include <deque>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <thread>
#include <vector>
#include <map>
#include <Windows.h>
#include <optional>
#include <unordered_map>

#include <algorithm>
#include <fnv1a.hpp>
#include <imgui.h>

// wah wah wah stop fucking crying bitch, i don't wanna hear it
#pragma warning(disable : 4018)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4477)

#define LOG(s, ...) printf(s "\n", __VA_ARGS__)

#define THROW(s) throw std::runtime_error(s)

using i8 = int8_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;

using long_t = long;
using llong_t = long long;
using ulong_t = unsigned long;
using ullong_t = unsigned long long;

using addr_t = u64;

template <typename T>
using UniquePtr = std::unique_ptr<T>;

template <typename T>
using SharedPtr = std::shared_ptr<T>;

struct V3 {
	f32 x{}, y{}, z{};
};

struct V2i {
	i32 x, y;

	__forceinline operator bool() const {
		return x == 0 && y == 0;
	}

	V2i operator+(i32 value) const {
		return V2i(x + value, y + value);
	}

	V2i operator-(i32 value) const {
		return V2i(x - value, y - value);
	}

	V2i operator/(i32 value) const {
		return V2i(x / value, y / value);
	}

	V2i operator+(const V2i& rhs) const {
		return V2i(x + rhs.x, y + rhs.y);
	}

	V2i operator-(const V2i& rhs) const {
		return V2i(x - rhs.x, y - rhs.y);
	}

	V2i operator/(const V2i& rhs) const {
		return V2i(x / rhs.x, y / rhs.y);
	}

	__forceinline operator ImVec2() const {
		return ImVec2((f32)x, (f32)y);
	}
};

struct V4i {
	i32 x{}, y{}, w{}, h{};

	V4i() = default;
	V4i(i32 x, i32 y, i32 w, i32 h) : x(x), y(y), w(w), h(h) { }
	V4i(const V2i& pos, const V2i& size) : x(pos.x), y(pos.y), w(size.x), h(size.y) { }

	operator bool() const {
		return x == y == w == h == 0;
	}

	V2i pos() const {
		return V2i{ x, y };
	}

	V2i size() const {
		return V2i{ w, h };
	}
};

struct Color {
	u32 data;

	Color() : data() {}
	Color(u32 data) : data(data) {}
	Color(u8 red, u8 green, u8 blue, u8 alpha) : data(IM_COL32(red, green, blue, alpha)) {}

	operator u32() const {
		return data;
	}

	Color subtract_alpha(u8 rhs) const {
		return Color((data & 0x00FFFFFF) | ((255 - rhs) << 24));
	}
};

class PerfomanceTimer {
public:
	__forceinline void start() {

	}

	__forceinline void tick() {
		const auto ms_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - m_last_fps).count();

		m_frame_counter++;
		if (ms_elapsed > 1000.f) {
			m_frames_per_second = m_frame_counter;
			m_frame_counter = 0;
			m_last_fps = std::chrono::high_resolution_clock::now();

		}

		m_last_frametime = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - m_last_frame).count() / 1000.f;

		m_last_frame = std::chrono::high_resolution_clock::now();

	}

	__forceinline i32 get_fps() {
		return m_frames_per_second;
	}

	__forceinline f32 get_last_frametime() {
		return m_last_frametime;
	}
private:
	std::chrono::high_resolution_clock::time_point m_last_fps, m_last_frame;

	int m_frame_counter;
	float m_frames_per_second = 0.f;
	float m_last_frametime = 0.f;
};

inline std::unordered_map<std::string, PerfomanceTimer> _perf_timers;