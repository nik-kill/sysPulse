#pragma once

#include "syspulse/metrics.hpp"
#include "syspulse/poller.hpp"

#include <cstddef>
#include <vector>

namespace syspulse {

/** Renders the live system telemetry dashboard on the ImGui thread. */
class DashboardUI {
public:
	explicit DashboardUI(Poller& poller, std::size_t history_limit = 120);

	void render();

private:
	void consume_snapshots();
	void add_history_sample(std::vector<float>& history, float value);
	void render_metric_panel();
	void render_history_panel();

	Poller& poller_;
	std::size_t history_limit_;
	SystemSnapshot latest_snapshot_{};
	std::vector<float> cpu_history_;
	std::vector<float> memory_history_;
	bool has_snapshot_{false};
};

} // namespace syspulse
