#include "syspulse/poller.hpp"
#include <thread>

namespace syspulse {
Poller::Poller(std::unique_ptr<ISystemCollector> collector, std::chrono::milliseconds interval, std::size_t queue_capacity)
	: collector_(std::move(collector)), interval_(interval), queue_(queue_capacity) {}
Poller::~Poller() { stop(); queue_.close(); }
void Poller::start() {
	if (worker_.joinable()) return;
	worker_ = std::jthread([this](std::stop_token token) { run(token); });
}
void Poller::stop() {
	if (worker_.joinable()) worker_.request_stop();
}
std::optional<SystemSnapshot> Poller::next_snapshot() { return queue_.wait_pop(); }
std::optional<SystemSnapshot> Poller::try_next_snapshot() { return queue_.try_pop(); }
void Poller::run(std::stop_token token) {
	while (!token.stop_requested()) {
		queue_.push(collector_->collect_snapshot());
		std::this_thread::sleep_for(interval_);
	}
}
} // namespace syspulse
