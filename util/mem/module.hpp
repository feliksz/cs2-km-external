#pragma once
#include <common.hpp>
#include "driver.hpp"
namespace mem {
	class Module {
	public:
		Module() : m_module_name("null"), m_base_address(0u), m_size(0u) { }
		Module(const std::string& module_name, addr_t base_address, size_t size) : m_module_name(module_name), m_base_address(base_address), m_size(size) { }

		bool is_valid() { return m_base_address > 0 && m_size > 0; }

		void read();

		addr_t find_pattern(const std::string& pattern, bool rip);

		std::string m_module_name;
		addr_t m_base_address;
		size_t m_size;

		byte* m_bytes{};
	};
}