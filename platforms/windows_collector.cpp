#include "windows_collector.hpp"

#ifndef _WIN32
#error "WindowsCollector requires Windows."
#endif

#include <windows.h>

namespace syspulse {


namespace {
std::uint64_t file_time_to_uint64(const FILETIME& value) {
	ULARGE_INTEGER converted{};
	converted.LowPart = value.dwLowDateTime;
	converted.HighPart = value.dwHighDateTime;
	return converted.QuadPart;
}
}

WindowsCollector::CpuTimes WindowsCollector::read_cpu_times() {
	FILETIME idle{}, kernel{}, user{};
	if (!GetSystemTimes(&idle, &kernel, &user)) {
		return {};
	}

	return {
		file_time_to_uint64(idle),
		file_time_to_uint64(kernel),
		file_time_to_uint64(user)
	};
}

CpuStats WindowsCollector::collect_cpu() {
	const auto current = read_cpu_times();
	CpuStats result{};

	if (!has_previous_cpu_sample_) {
		previous_cpu_times_ = current;
		has_previous_cpu_sample_ = true;
		return result;
	}

	const auto idle_delta = current.idle >= previous_cpu_times_.idle
		? current.idle - previous_cpu_times_.idle
		: 0;
	const auto kernel_delta = current.kernel >= previous_cpu_times_.kernel
		? current.kernel - previous_cpu_times_.kernel
		: 0;
	const auto user_delta = current.user >= previous_cpu_times_.user
		? current.user - previous_cpu_times_.user
		: 0;
	const auto total_delta = kernel_delta + user_delta;
	const auto active_delta = total_delta >= idle_delta ? total_delta - idle_delta : 0;

	if (total_delta != 0) {
		result.overall_percent = 100.0 * static_cast<double>(active_delta) /
			static_cast<double>(total_delta);
	}

	result.user_ticks = current.user;
	result.system_ticks = current.kernel >= current.idle ? current.kernel - current.idle : 0;
	result.idle_ticks = current.idle;
	previous_cpu_times_ = current;
	return result;
}

MemoryStats WindowsCollector::collect_memory() {
	MEMORYSTATUSEX status{};
	status.dwLength = sizeof(status);
	if (!GlobalMemoryStatusEx(&status)) {
		return {};
	}

	const auto used_memory = status.ullTotalPhys >= status.ullAvailPhys
		? status.ullTotalPhys - status.ullAvailPhys
		: 0;
	const auto used_swap = status.ullTotalPageFile >= status.ullAvailPageFile
		? status.ullTotalPageFile - status.ullAvailPageFile
		: 0;

	return {
		status.ullTotalPhys,
		used_memory,
		status.ullAvailPhys,
		status.ullTotalPageFile,
		used_swap
	};
}

std::vector<DiskStats> WindowsCollector::collect_disks() { return {}; }
std::vector<NetworkStats> WindowsCollector::collect_networks() { return {}; }
}
