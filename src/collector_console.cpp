#include "windows_collector.hpp"

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <thread>

namespace {
void print_snapshot(const syspulse::CpuStats& cpu, const syspulse::MemoryStats& memory) {
	const auto memory_gib = [](std::uint64_t bytes) {
		return static_cast<double>(bytes) / (1024.0 * 1024.0 * 1024.0);
	};

	std::cout << "CPU: " << std::fixed << std::setprecision(1)
		<< cpu.overall_percent << "% | Memory: "
		<< memory_gib(memory.used_bytes) << " / "
		<< memory_gib(memory.total_bytes) << " GiB ("
		<< (memory.total_bytes == 0 ? 0.0
			: 100.0 * static_cast<double>(memory.used_bytes) /
				static_cast<double>(memory.total_bytes))
		<< "%) | Available: " << memory_gib(memory.free_bytes)
		<< " GiB" << std::endl;
}
}

int main() {
	std::cout << "SysPulse Windows collector v0.2.0\n"
		<< "Press Ctrl+C to stop.\n\n";

	syspulse::WindowsCollector collector;
	while (true) {
		const auto cpu = collector.collect_cpu();
		const auto memory = collector.collect_memory();
		print_snapshot(cpu, memory);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
