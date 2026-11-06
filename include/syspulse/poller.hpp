#pragma once

#include "syspulse/bounded_queue.hpp"
#include "syspulse/collector.hpp"

#include <chrono>
#include <memory>
#include <optional>
#include <thread>

namespace syspulse {

class Poller {
public:
	using Queue = BoundedQueue<SystemSnapshot>;

	Poller(std::unique_ptr<ISystemCollector> collector,
		   std::chrono::milliseconds interval = std::chrono::seconds(1),
		   std::size_t queue_capacity = 8);
	~Poller();

	Poller(const Poller&) = delete;
	Poller& operator=(const Poller&) = delete;

	void start();
	void stop();
	std::optional<SystemSnapshot> next_snapshot();

private:
	void run(std::stop_token token);
	std::unique_ptr<ISystemCollector> collector_;
	std::chrono::milliseconds interval_;
	Queue queue_;
	std::jthread worker_;
};

} // namespace syspulse
