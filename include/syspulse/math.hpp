#pragma once

#include <cstdint>

namespace syspulse {

/** Returns a non-negative counter rate, protecting against reset and overflow. */
double counter_rate(std::uint64_t previous, std::uint64_t current, double elapsed_seconds);

/** Updates an exponential moving average and returns its new value. */
double exponential_moving_average(double previous, double sample, double alpha);

/** Converts active and idle counter deltas to a percentage in [0, 100]. */
double utilization_percent(std::uint64_t active_delta, std::uint64_t idle_delta);

} // namespace syspulse
