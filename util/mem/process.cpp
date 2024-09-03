#include <ranges>
#include <util/winapi/winapi.hpp>
#include "driver.hpp"
#include <TlHelp32.h>

#include "process.hpp"

using namespace mem::process::_;

bool mem::process::find(const std::string& process_name) {
    auto snapshot = winapi::create_snapshot()(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return false;

    winapi::ProcessEntry entry{};
    entry.size = sizeof(PROCESSENTRY32);

    winapi::process_first()(snapshot, &entry);
    while (winapi::process_next()(snapshot, &entry)) {
        if (strcmp(entry.exe_file, process_name.data()) == 0) {
            _process_id = entry.process_id;
            break;
        }
    }

    winapi::close_handle()(snapshot);
    return _process_id != 0;
}

bool mem::process::init() {
    if (!driver::init_process(_process_id, &_peb_address))
        THROW("couldn't initialize process");
    
    LOG("[process] attached. (peb address: 0x%llx)", _peb_address);

    return true;
}

std::string mem::process::read_string(addr_t address, size_t length) {
    if (!address)
        return "fail2";

    auto buf = std::string(length, 0);
    if (!driver::read_memory(address, buf.data(), length))
        return "fail";

    if (const auto it = std::ranges::find(buf, '\0'); it != buf.end())
        buf.erase(it, buf.end());

    return buf;
}

std::string mem::process::read_unicode_string(const UNICODE_STRING& str) {
    auto wbuf = std::wstring(str.Length, 0);
    if (!driver::read_memory((u64)str.Buffer, wbuf.data(), str.Length))
        return "fail";  

    auto buf = std::string(wbuf.begin(), wbuf.end());
    if (const auto it = std::ranges::find(buf, '\0'); it != buf.end())
        buf.erase(it, buf.end());

    return buf;
}

std::vector<mem::Module> mem::process::get_all_modules() {
    constexpr static auto PEB_LDR_OFFSET = 0x18;
    constexpr static auto PEB_LDR_MODULE_LIST_OFFSET = 0x10;

    std::vector<mem::Module> ret;

    auto peb_ldr = driver::read_memory<>(_peb_address + PEB_LDR_OFFSET);
    auto in_load_order_module_list = driver::read_memory<LIST_ENTRY>(peb_ldr + PEB_LDR_MODULE_LIST_OFFSET);

    auto list_head = (u64)in_load_order_module_list.Flink;
    auto list_node = list_head;
    do {
        auto entry = driver::read_memory<LDR_DATA_TABLE_ENTRY>(list_node);
        list_node = (u64)entry.InLoadOrderModuleList.Flink;

        if (!entry.BaseAddress)
            continue;

        auto module_name = read_unicode_string(entry.BaseDllName);
        ret.push_back(mem::Module(module_name, (u64)entry.BaseAddress, entry.SizeOfImage));

    } while (list_node && list_node != list_head);

    return ret;
}


mem::Module mem::process::find_module(std::string module_name) {
    for (auto& m : get_all_modules()) {
        if (module_name == m.m_module_name) {
            return m;
        }
    }

    return mem::Module();
}