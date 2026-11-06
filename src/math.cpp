#include "syspulse/math.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace syspulse {
double counter_rate(std::uint64_t previous, std::uint64_t current, double elapsed_seconds) {
	if (elapsed_seconds <= 0.0 || current < previous) return 0.0;
	return static_cast<double>(current - previous) / elapsed_seconds;
}
double exponential_moving_average(double previous, double sample, double alpha) {
	alpha = std::clamp(alpha, 0.0, 1.0);
	return previous + alpha * (sample - previous);
}
double utilization_percent(std::uint64_t active_delta, std::uint64_t idle_delta) {
	const auto total = active_delta + idle_delta;
	return total == 0 ? 0.0 : std::clamp(100.0 * static_cast<double>(active_delta) / static_cast<double>(total), 0.0, 100.0);
}
} // namespace syspulse
