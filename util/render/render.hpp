#pragma once
#include <d3d11.h>
#include <common.hpp>

#pragma comment(lib, "d3d11.lib")

constexpr auto ROUND_TOP_LEFT = ImDrawFlags_RoundCornersTopLeft;
constexpr auto ROUND_TOP_RIGHT = ImDrawFlags_RoundCornersTopRight;
constexpr auto ROUND_BOTTOM_LEFT = ImDrawFlags_RoundCornersBottomLeft;
constexpr auto ROUND_BOTTOM_RIGHT = ImDrawFlags_RoundCornersBottomRight;

constexpr auto ROUND_LEFT = ImDrawFlags_RoundCornersLeft;
constexpr auto ROUND_TOP = ImDrawFlags_RoundCornersTop;
constexpr auto ROUND_RIGHT = ImDrawFlags_RoundCornersRight;
constexpr auto ROUND_BOTTOM = ImDrawFlags_RoundCornersBottom;

constexpr auto ROUND_ALL = ImDrawFlags_RoundCornersAll;


namespace render {
	namespace _ {
		inline IDXGISwapChain* _swap_chain;
		inline ID3D11Device* _device;
		inline ID3D11DeviceContext* _device_ctx;
		inline ID3D11RenderTargetView* _render_target_view;

		inline ImDrawList* _draw_list;

		bool create_d3d_device(HWND hwnd, i32 refresh_rate);

		void create_render_target();
		void clear_render_target();
	}

	void init(HWND hwnd, i32 refresh_rate);

	void begin();
	void end();

	void rect(const V2i& pos, const V2i& size, const Color& color, float rounding = 0.f, i32 flags = 0);
	void fill(const V2i& pos, const V2i& size, const Color& color, float rounding = 0.f, i32 flags = 0);
	void line(const V2i& start, const V2i& end, const Color& color, f32 thickness = 1.f);
	void text(const V2i& pos, const Color& color, const std::string& str);

	V2i get_text_size(const std::string& str);
}