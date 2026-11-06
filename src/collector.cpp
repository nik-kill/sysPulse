#include "syspulse/collector.hpp"

#include <utility>

namespace syspulse {
SystemSnapshot ISystemCollector::collect_snapshot() {
	return {std::chrono::system_clock::now(), collect_cpu(), collect_memory(), collect_disks(), collect_networks()};
}
}
