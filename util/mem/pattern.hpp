#pragma once
#include <common.hpp>

namespace mem {
	using signature_t = std::vector<byte>;

	struct pattern_t {
		std::vector<byte> bytes;

		pattern_t() = default;
		pattern_t(const std::string& pattern) {
			auto iss = std::istringstream(pattern);
			auto chunks = std::vector<std::string>{ 
								std::istream_iterator<std::string>{iss}, 
								std::istream_iterator<std::string>{} };

			for (auto& b : chunks) {
				bytes.push_back(std::strtoul(b.data(), 0, 16));
			}
		}
	};
}