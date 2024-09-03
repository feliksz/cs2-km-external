#pragma once
#include <common.hpp>

namespace driver {
	namespace _ {
		struct DriverRequest {
			enum RequestType : i64 {
				INIT, READ
			};

			RequestType type;

			union {
				struct InitRequest { i64 process_id; } init;
				struct ReadRequest { u64 address; } read;
			};
		};
		inline void* _device;
	};

	void init(const char* service_name);

	bool init_process(u32 process_id, addr_t* peb_address);
	bool read_memory(u64 address, void* buffer, size_t length);

	void send_mouse_state(u32 flags, u32 x = 0, u32 y = 0);

	template <typename T = u64> __forceinline T read_memory(addr_t address) {
		T buf;
		if (!read_memory(address, &buf, sizeof(buf))) {
			return T{};
		}

		return buf;
	}

	addr_t resolve_rip(u64 address);

	addr_t find_pattern(byte* data, size_t data_len, const std::string& pattern);


}