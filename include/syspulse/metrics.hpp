#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace syspulse {

struct CpuStats {
	double overall_percent{};
	std::vector<double> per_core_percent;
	std::uint64_t user_ticks{};
	std::uint64_t system_ticks{};
	std::uint64_t idle_ticks{};
};

struct MemoryStats {
	std::uint64_t total_bytes{};
	std::uint64_t used_bytes{};
	std::uint64_t free_bytes{};
	std::uint64_t swap_total_bytes{};
	std::uint64_t swap_used_bytes{};
};

struct DiskStats {
	std::string mount_point;
	std::uint64_t total_bytes{};
	std::uint64_t available_bytes{};
	double read_bytes_per_second{};
	double write_bytes_per_second{};
};

struct NetworkStats {
	std::string interface_name;
	std::uint64_t received_bytes{};
	std::uint64_t transmitted_bytes{};
	std::uint64_t received_packets{};
	std::uint64_t transmitted_packets{};
	double received_bytes_per_second{};
	double transmitted_bytes_per_second{};
};

struct SystemSnapshot {
	std::chrono::system_clock::time_point timestamp;
	CpuStats cpu;
	MemoryStats memory;
	std::vector<DiskStats> disks;
	std::vector<NetworkStats> networks;
};

} // namespace syspulse
