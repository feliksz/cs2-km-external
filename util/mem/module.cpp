#include "module.hpp"
#include "pattern.hpp"

void mem::Module::read() {
	//if (!is_valid())
	//	return;

	m_bytes = new byte[m_size];
	driver::read_memory(m_base_address, m_bytes, m_size);
}

addr_t mem::Module::find_pattern(const std::string& pattern, bool rip) {
	read();

	if (auto result = driver::find_pattern(m_bytes, m_size, pattern))
		return rip ? driver::resolve_rip(m_base_address + result) : m_base_address + result;

	delete m_bytes;
	return 0;
}
