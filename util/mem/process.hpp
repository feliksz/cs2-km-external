#pragma once
#include <common.hpp>
#include "module.hpp"

#include <util/winapi/native.hpp>
namespace mem::process {
	namespace _ {
		inline u32 _process_id;
		inline addr_t _peb_address;
	}

	bool find(const std::string& process_name);
	bool init();

	inline bool read_memory(u64 address, void* buffer, size_t length) {
		return driver::read_memory(address, buffer, length);
	}

	template <typename T = u64> __forceinline T read_memory(addr_t address) {
		T buf;
		if (!driver::read_memory(address, &buf, sizeof(buf))) {
			return T{};
		}

		return buf;
	}

	std::string read_string(addr_t address, size_t length);
	std::string read_unicode_string(const UNICODE_STRING& str);

	std::vector<mem::Module> get_all_modules();
	mem::Module find_module(std::string module_name);
}

namespace process {
	using namespace mem::process;
}
