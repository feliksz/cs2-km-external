#include "schemas.hpp"

#include <fstream>
void schemas::init(cstrike::SchemaSystem* schema_system) {
	for (auto& ts : schema_system->get_type_scopes()) {
		if (ts->get_module_name() != "client.dll")
			continue;
		for (auto& cl : ts->get_classes()) {
			if (!cl)
				continue;

			for (auto& field : cl->get_fields()) {
				if (!field)
					continue;

				_::_offsets[cl->get_module_name()][field->get_name()] = field->get_offset();
			}
		}
	}

	std::ofstream ofs("offsets.odmp", std::ofstream::out);
	for (auto& cl : _::_offsets) {
		for (auto& field : cl.second) {
			ofs << cl.first << "::" << field.first << " -> " << field.second << std::endl;
		}
	}

	ofs.close();
}
