#include "dashboard_ui.hpp"

#include "imgui.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>

namespace syspulse {
namespace {
float memory_percent(const MemoryStats& memory) {
	if (memory.total_bytes == 0) return 0.0F;
	return std::clamp(
		100.0F * static_cast<float>(memory.used_bytes) /
			static_cast<float>(memory.total_bytes),
		0.0F,
		100.0F);
}

float to_gib(std::uint64_t bytes) {
	return static_cast<float>(bytes) / (1024.0F * 1024.0F * 1024.0F);
}
}

DashboardUI::DashboardUI(Poller& poller, std::size_t history_limit)
	: poller_(poller), history_limit_(std::max<std::size_t>(history_limit, 2)) {
	cpu_history_.reserve(history_limit_);
	memory_history_.reserve(history_limit_);
}

void DashboardUI::consume_snapshots() {
	while (const auto snapshot = poller_.try_next_snapshot()) {
		latest_snapshot_ = *snapshot;
		has_snapshot_ = true;
		add_history_sample(cpu_history_, static_cast<float>(latest_snapshot_.cpu.overall_percent));
		add_history_sample(memory_history_, memory_percent(latest_snapshot_.memory));
	}
}

void DashboardUI::add_history_sample(std::vector<float>& history, float value) {
	if (history.size() >= history_limit_) {
		history.erase(history.begin());
	}
	history.push_back(value);
}

void DashboardUI::render_metric_panel() {
	if (!has_snapshot_) {
		ImGui::TextUnformatted("Waiting for the first telemetry sample...");
		return;
	}

	const auto& cpu = latest_snapshot_.cpu;
	const auto& memory = latest_snapshot_.memory;
	const auto memory_usage = memory_percent(memory);

	ImGui::TextUnformatted("Current metrics");
	ImGui::Separator();
	ImGui::Text("CPU utilization: %.1f%%", cpu.overall_percent);
	ImGui::ProgressBar(static_cast<float>(cpu.overall_percent / 100.0), ImVec2(-1.0F, 0.0F), "CPU");
	ImGui::Spacing();
	ImGui::Text("Memory: %.2f / %.2f GiB (%.1f%%)",
		to_gib(memory.used_bytes), to_gib(memory.total_bytes), memory_usage);
	ImGui::ProgressBar(memory_usage / 100.0F, ImVec2(-1.0F, 0.0F), "Memory");
	ImGui::Spacing();
	ImGui::Text("Available memory: %.2f GiB", to_gib(memory.free_bytes));
	ImGui::Text("Pagefile used: %.2f / %.2f GiB",
		to_gib(memory.swap_used_bytes), to_gib(memory.swap_total_bytes));
}

void DashboardUI::render_history_panel() {
	ImGui::TextUnformatted("History (last 120 samples)");
	ImGui::Separator();
	if (cpu_history_.size() > 1) {
		ImGui::PlotLines("CPU %", cpu_history_.data(), static_cast<int>(cpu_history_.size()), 0, nullptr, 0.0F, 100.0F, ImVec2(-1.0F, 90.0F));
	} else {
		ImGui::TextUnformatted("Collecting CPU history...");
	}
	if (memory_history_.size() > 1) {
		ImGui::PlotLines("Memory %", memory_history_.data(), static_cast<int>(memory_history_.size()), 0, nullptr, 0.0F, 100.0F, ImVec2(-1.0F, 90.0F));
	} else {
		ImGui::TextUnformatted("Collecting memory history...");
	}
}

void DashboardUI::render() {
	consume_snapshots();

	ImGui::SetNextWindowSize(ImVec2(620.0F, 520.0F), ImGuiCond_FirstUseEver);
	ImGui::Begin("SysPulse Dashboard");
	ImGui::TextUnformatted("Windows system health monitor - v0.3.0");
	ImGui::Spacing();
	render_metric_panel();
	ImGui::Spacing();
	render_history_panel();
	ImGui::End();
}

} // namespace syspulse
