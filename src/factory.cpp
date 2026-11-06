#include "syspulse/collector.hpp"
#include "windows_collector.hpp"

#ifndef _WIN32
#error "SysPulse is a Windows-only application."
#endif

namespace syspulse {
std::unique_ptr<ISystemCollector> create_system_collector() {
	return std::make_unique<WindowsCollector>();
}
std::string platform_name() {
	return "Windows";
}
}
