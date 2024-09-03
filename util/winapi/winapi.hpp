#pragma once
#include <common.hpp>
#include <intrin.h>

#include "native.hpp"

#include <dwmapi.h>
#define WINAPI_EXPORT(func_type, name, module_name, export_name, ...) namespace _ { \
	using name##_t = func_type(*)(__VA_ARGS__); } \
	inline _::name##_t name() { \
		static auto fn_##name = winapi::_::find_export(module_name, export_name); \
		return (_::name##_t)fn_##name; \
	} \

namespace winapi {
	namespace _ {
		void* find_export(const std::string& module_name, const std::string& export_name);
	}

	namespace th32 { // e chcialem nie includowac z windowsa potem to wykurwie na zbity pysk pzdr
		struct ProcessEntry {
			ulong_t size, usage, process_id;
			u64 default_heap_id;
			ulong_t module_id, num_threads, parent_process_id;
			long_t priority_class_base;
			ulong_t flags;
			char exe_file[MAX_PATH];
		};

		constexpr inline ulong_t TH32_SNAPPROCESS = 0x2;
		constexpr inline ulong_t TH32_SNAPMODULE64 = 0x08;
		constexpr inline ulong_t TH32_SNAPMODULE32 = 0x10;
	}

	using namespace th32;

	// advapi32.dll
	WINAPI_EXPORT(void*, open_service_manager, "advapi32.dll", "OpenSCManagerA", const char*, const char*, u32);
	WINAPI_EXPORT(void*, open_service, "advapi32.dll", "OpenServiceA", void*, const char*, u32);
	WINAPI_EXPORT(bool, start_service, "advapi32.dll", "StartServiceA", void*, u32, const char**);
	WINAPI_EXPORT(bool, close_service_handle, "advapi32.dll", "CloseServiceHandle", void*);


	// kernel32.dll
	WINAPI_EXPORT(bool, device_io_control, "kernel32.dll", "DeviceIoControl", void*, u32, void*, u32, void*, u32, u32, void*);
	WINAPI_EXPORT(void*, create_file, "kernel32.dll", "CreateFileA", const char*, u32, u32, void*, u32, u32, void*);
	WINAPI_EXPORT(void*, create_snapshot, "kernel32.dll", "CreateToolhelp32Snapshot", u32, u32);
	WINAPI_EXPORT(bool, process_first, "kernel32.dll", "Process32First", void*, th32::ProcessEntry*);
	WINAPI_EXPORT(bool, process_next, "kernel32.dll", "Process32Next", void*, th32::ProcessEntry*);
	WINAPI_EXPORT(bool, close_handle, "kernel32.dll", "CloseHandle", void*);
	WINAPI_EXPORT(u32, get_last_error, "kernel32.dll", "GetLastError");

	// user32.dll
	WINAPI_EXPORT(HWND, create_window, "user32.dll", "CreateWindowExA", u32, const char*, const char*, u32, i32, i32, i32, i32, HWND, HMENU, HINSTANCE, void*);
	WINAPI_EXPORT(bool, show_window, "user32.dll", "ShowWindow", HWND, i32);
	WINAPI_EXPORT(bool, update_window, "user32.dll", "UpdateWindow", HWND);
	WINAPI_EXPORT(bool, destroy_window, "user32.dll", "DestroyWindow", HWND);
	
	WINAPI_EXPORT(bool, peek_message, "user32.dll", "PeekMessageA", LPMSG, HWND, u32, u32, u32);
	WINAPI_EXPORT(bool, translate_message, "user32.dll", "TranslateMessage", const MSG*);
	WINAPI_EXPORT(i64, dispatch_message, "user32.dll", "DispatchMessageA", const MSG*);
	WINAPI_EXPORT(bool, set_layered_window_attributes, "user32.dll", "SetLayeredWindowAttributes", HWND, u32, u8, u32);

	WINAPI_EXPORT(u16, register_class, "user32.dll", "RegisterClassExA", WNDCLASSEXA*);
	WINAPI_EXPORT(bool, unregister_class, "user32.dll", "UnregisterClass", const char*, HINSTANCE);

	WINAPI_EXPORT(i64, def_window_proc, "user32.dll", "DefWindowProcA", HWND, u32, u64, i64);
	WINAPI_EXPORT(HDC, get_dc, "user32.dll", "GetDC", HWND);
	WINAPI_EXPORT(i32, release_dc, "user32.dll", "ReleaseDC", HWND, HDC);

	WINAPI_EXPORT(i32, get_system_metrics, "user32.dll", "GetSystemMetrics", i32);

	// gdi32.dll
	WINAPI_EXPORT(i32, get_device_caps, "gdi32.dll", "GetDeviceCaps", HDC, i32);

	// dwmapi.dll
	WINAPI_EXPORT(i64, dwm_extend_frame_into_client_area, "dwmapi.dll", "DwmExtendFrameIntoClientArea", HWND, const MARGINS*);
}