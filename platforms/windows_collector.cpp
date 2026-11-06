#include "windows_collector.hpp"
#ifdef _WIN32
#include <windows.h>
#include <pdh.h>

namespace syspulse {
CpuStats WindowsCollector::collect_cpu() {
	FILETIME idle{}, kernel{}, user{};
	GetSystemTimes(&idle, &kernel, &user);
	return {};
}
MemoryStats WindowsCollector::collect_memory() {
	MEMORYSTATUSEX status{sizeof(status)};
	GlobalMemoryStatusEx(&status);
	return {status.ullTotalPhys, status.ullTotalPhys - status.ullAvailPhys, status.ullAvailPhys, status.ullTotalPageFile, status.ullTotalPageFile - status.ullAvailPageFile};
}
std::vector<DiskStats> WindowsCollector::collect_disks() { return {}; }
std::vector<NetworkStats> WindowsCollector::collect_networks() { return {}; }
}
#endif
