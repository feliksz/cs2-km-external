#include "overlay.hpp"
#include <imgui.h>
#include <imgui_impl_win32.h>

using namespace overlay::_;

constexpr static auto OVERLAY_WINDOW_FLAGS = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT;

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

i64 __stdcall wnd_proc(HWND hwnd, u32 msg, u64 wp, i64 lp) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wp, lp)) {
        return 0;
    }

	ImGuiIO& io = ImGui::GetIO();
	switch (msg) {
	case WM_ACTIVATE:
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		return 0;

	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		return true;

	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		return true;

	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		return true;

	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wp) > 0 ? +1.0f : -1.0f;
		return true;

	case WM_SYSCOMMAND:
		if ((wp & 0xfff0) == SC_KEYMENU)
			return 0;
		break;

	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lp);
		io.MousePos.y = (signed short)(lp >> 16);
		return 0;

	case WM_KEYDOWN:
		if (wp < 256)
			io.KeysDown[wp] = 1;
		return true;

	case WM_KEYUP:
		if (wp < 256)
			io.KeysDown[wp] = 0;
		return true;

	case WM_CHAR:
		if (wp > 0 && wp < 0x10000)
			io.AddInputCharacter((unsigned short)wp);
		return true;
	}

    return winapi::def_window_proc()(hwnd, msg, wp, lp);
}

void overlay::init(HWND target, const std::string& title) {
    _wc.cbClsExtra = 0;
    _wc.cbWndExtra = 0;
    _wc.style = CS_HREDRAW | CS_VREDRAW;
    _wc.cbSize = sizeof(WNDCLASSEX);
    _wc.hCursor = LoadCursorA(0, IDC_ARROW);
    _wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    _wc.hIcon = _wc.hIconSm = LoadIconA(0, IDI_APPLICATION);

    _wc.lpfnWndProc = wnd_proc;
    _wc.lpszClassName = _wc.lpszMenuName = title.data();

    winapi::register_class()(&_wc);

	ImGui::CreateContext();


    _screen_size = V2i{ winapi::get_system_metrics()(SM_CXSCREEN), winapi::get_system_metrics()(SM_CYSCREEN) };

    const auto screen_size = get_screen_size();
    _hwnd = winapi::create_window()(OVERLAY_WINDOW_FLAGS, title.data(), title.data(), WS_POPUP, 0, 0, screen_size.x, screen_size.y, 0, 0, _wc.hInstance, 0);

    winapi::set_layered_window_attributes()(_hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    const auto margins = MARGINS{ -1,-1,-1,-1 };
    winapi::dwm_extend_frame_into_client_area()(_hwnd, &margins);

    winapi::show_window()(_hwnd, SW_SHOW);
    winapi::update_window()(_hwnd);

    _is_running = true;
}

void overlay::release() {
    winapi::destroy_window()(_hwnd);
    winapi::unregister_class()(_wc.lpszClassName, _wc.hInstance);
}

bool overlay::run() {
	MSG msg{};
	if (winapi::peek_message()(&msg, 0, 0, 0, PM_REMOVE)) {
		winapi::translate_message()(&msg);
		winapi::dispatch_message()(&msg);
	}

	if (msg.message == WM_QUIT)
		return false;

	_screen_size = V2i{ winapi::get_system_metrics()(SM_CXSCREEN), winapi::get_system_metrics()(SM_CYSCREEN) };

    return true;
}

V2i overlay::get_screen_size() {
    return _screen_size;
}

i32 overlay::get_screen_refresh_rate() {
    auto dc = winapi::get_dc()(_hwnd);
    auto refresh_rate = winapi::get_device_caps()(dc, VREFRESH);
    winapi::release_dc()(_hwnd, dc);
    return refresh_rate;
}
