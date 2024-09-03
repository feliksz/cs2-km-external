#include "../winapi/winapi.hpp"
#include "driver.hpp"
#include "pattern.hpp"

using namespace driver::_;

#define IOCTL_DRIVER_METHOD CTL_CODE(40000, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS)

void driver::init(const char* service_name) {
	const auto sc_manager = winapi::open_service_manager()(0, 0, SC_MANAGER_ALL_ACCESS);
	if (!sc_manager)
		THROW("couldn't open service manager");

	LOG("[driver] sc_manager -> 0x%p", sc_manager);

	const auto sc_service = winapi::open_service()(sc_manager, service_name, SERVICE_ALL_ACCESS);
	if (!sc_service)
		THROW("couldn't open service");

	LOG("[driver] sc_service -> 0x%p", sc_service);

	if (!winapi::start_service()(sc_service, 0, 0)) {
		if (winapi::get_last_error()() != ERROR_SERVICE_ALREADY_RUNNING)
			THROW("couldn't start service");
	}

	// sorry not closing services handles because winapi "wrapper" i wrote is trash asf, windows will take care of handles (please)

	auto drv_filename = std::format("\\\\.\\{}", service_name);
	_device = winapi::create_file()(
					drv_filename.data(), 
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ,
					0,
					OPEN_EXISTING, 
					FILE_ATTRIBUTE_NORMAL,
					0);

	if (_device == INVALID_HANDLE_VALUE)
		THROW("couldn't connect to the driver");

	LOG("[driver] device: %llu", (u64)_device);
}

addr_t driver::find_pattern(byte* data, size_t data_len, const std::string& pattern) {
	const auto pat = mem::pattern_t(pattern);

	auto& signature = pat.bytes;
	for (auto i = 0; i < data_len - signature.size(); ++i) {
		bool found = true;
		for (auto j = 0; j < signature.size(); ++j) {
			if (data[i + j] != signature[j] && signature[j] != 0ui8) {
				found = false;
				break;
			}
		}

		if (found) {
			return i;
		}
	}

	return addr_t();
}

addr_t driver::resolve_rip(u64 address) {
	return address != 0 ? address + read_memory<i32>(address + 3) + 7 : 0;
}

bool driver::init_process(u32 process_id, addr_t* peb_address) {
	DriverRequest req;
	req.type = DriverRequest::INIT;
	req.init.process_id = process_id;
	return DeviceIoControl(_device, IOCTL_DRIVER_METHOD, &req, sizeof(req), peb_address, sizeof(addr_t), 0, 0);
}

bool driver::read_memory(u64 address, void* buffer, size_t length) {
	DriverRequest req;
	req.type = DriverRequest::READ;
	req.read.address = address;
	return DeviceIoControl(_device, IOCTL_DRIVER_METHOD, &req, sizeof(req), buffer, length, 0, 0);
}

void driver::send_mouse_state(u32 flags, u32 x, u32 y) {
	mouse_event(flags, x, y, 0, 0);
}
