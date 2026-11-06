#pragma once

#include "syspulse/collector.hpp"

#include <cstdint>

namespace syspulse {

/** Collects system metrics through Windows Win32 APIs. */
class WindowsCollector final : public ISystemCollector {
public:
	CpuStats collect_cpu() override;
	MemoryStats collect_memory() override;
	std::vector<DiskStats> collect_disks() override;
	std::vector<NetworkStats> collect_networks() override;

private:
	struct CpuTimes {
		std::uint64_t idle{};
		std::uint64_t kernel{};
		std::uint64_t user{};
	};

	static CpuTimes read_cpu_times();

	CpuTimes previous_cpu_times_{};
	bool has_previous_cpu_sample_{false};
};

} // namespace syspulse
