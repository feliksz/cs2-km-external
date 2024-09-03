#include <common.hpp>
#include "winapi.hpp"

void* winapi::_::find_export(const std::string& module_name, const std::string& export_name) {
	static std::unordered_map<std::string, HMODULE> modules;

	// if module not found in map get it or throw error lol
	if (modules.find(module_name) == modules.end()) {
		auto handle = GetModuleHandleA(module_name.data());
		if (!handle)
			handle = LoadLibraryA(module_name.data());

		if (!handle)
			THROW("couldn't load exports");

		LOG("[winapi][modules] %s -> 0x%llx", module_name.data(), (u64)handle);
		modules[module_name] = handle;
	}

	const auto handle = modules[module_name];
	const auto proc_address = GetProcAddress(handle, export_name.data());
	if (!proc_address)
		THROW("couldn't load exports (2)");


	LOG("[winapi] %s::%s -> 0x%llx", module_name.data(), export_name.data(), (u64)proc_address);
	return proc_address;
}
