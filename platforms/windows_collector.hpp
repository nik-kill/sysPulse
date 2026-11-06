#pragma once
#include "syspulse/collector.hpp"
namespace syspulse {
class WindowsCollector final : public ISystemCollector {
public:
	CpuStats collect_cpu() override;
	MemoryStats collect_memory() override;
	std::vector<DiskStats> collect_disks() override;
	std::vector<NetworkStats> collect_networks() override;
};
} // namespace syspulse
