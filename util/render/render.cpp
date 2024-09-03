#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include "render.hpp"

using namespace render::_;

bool render::_::create_d3d_device(HWND hwnd, i32 refresh_rate) {
    const D3D_FEATURE_LEVEL feature_lvl_arr[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    D3D_FEATURE_LEVEL feature_lvl;

    DXGI_SWAP_CHAIN_DESC scd{};
    scd.OutputWindow = hwnd;
    scd.Windowed = true;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.BufferCount = 2;
    scd.BufferDesc.Width = scd.BufferDesc.Height = 0;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = (u32)refresh_rate;
    scd.BufferDesc.RefreshRate.Denominator = 1;

    auto hr = D3D11CreateDeviceAndSwapChain(
        0, D3D_DRIVER_TYPE_HARDWARE,
        0,
        0,
        feature_lvl_arr,
        2,
        D3D11_SDK_VERSION,
        &scd,
        &_swap_chain,
        &_device,
        &feature_lvl,
        &_device_ctx);

    return hr == S_OK;
}

void render::_::create_render_target() {
    ID3D11Texture2D* backbuffer;
    _swap_chain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
    if (!backbuffer)
        THROW("couldn't create render target!");

    _device->CreateRenderTargetView(backbuffer, 0, &_render_target_view);
    backbuffer->Release();
}

void render::_::clear_render_target() {
    constexpr static float clear_color[] = { 0.f, 0.f, 0.f, 0.0f };
    _device_ctx->OMSetRenderTargets(1, &_render_target_view, 0);
    _device_ctx->ClearRenderTargetView(_render_target_view, clear_color);
}

void render::init(HWND hwnd, i32 refresh_rate) {
    if (!create_d3d_device(hwnd, refresh_rate))
        THROW("couldn't create d3d11 device.");

    create_render_target();

    auto& io = ImGui::GetIO();
    io.BackendPlatformUserData = nullptr;
    io.IniFilename = io.LogFilename = nullptr;


    auto font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 16.f, 0, io.Fonts->GetGlyphRangesDefault());

    io.Fonts->Build();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(_device, _device_ctx);
}

void render::begin() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    _draw_list = ImGui::GetBackgroundDrawList();
}

void render::end() {
    ImGui::Render();

    clear_render_target();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    _swap_chain->Present(0, 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(3));
}

void render::rect(const V2i& pos, const V2i& size, const Color& color, float rounding, i32 flags) {
    _draw_list->AddRect(pos, V2i{ pos.x + size.x, pos.y + size.y }, color, rounding, flags);
}

void render::fill(const V2i& pos, const V2i& size, const Color& color, float rounding, i32 flags) {
    _draw_list->AddRectFilled(pos, V2i{ pos.x + size.x, pos.y + size.y }, color, rounding, flags);
}

void render::line(const V2i& start, const V2i& end, const Color& color, f32 thickness) {
    _draw_list->AddLine(start, end, color, 1.f);
}

void render::text(const V2i& pos, const Color& color, const std::string& str) {
    _draw_list->AddText(pos, color, str.data());
}

V2i render::get_text_size(const std::string& str) {
    auto text_size = ImGui::CalcTextSize(str.data());
    return V2i{ (i32)text_size.x, (i32)text_size.y };
}
