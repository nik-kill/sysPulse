#pragma once

#include "syspulse/metrics.hpp"

#include <memory>
#include <string>

namespace syspulse {

class ISystemCollector {
public:
	virtual ~ISystemCollector() = default;
	virtual CpuStats collect_cpu() = 0;
	virtual MemoryStats collect_memory() = 0;
	virtual std::vector<DiskStats> collect_disks() = 0;
	virtual std::vector<NetworkStats> collect_networks() = 0;
	virtual SystemSnapshot collect_snapshot();
};

std::unique_ptr<ISystemCollector> create_system_collector();
std::string platform_name();

} // namespace syspulse
