#pragma once

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>
#include <utility>

namespace syspulse {

template <typename T>
class BoundedQueue {
public:
	explicit BoundedQueue(std::size_t capacity) : capacity_(capacity) {}

	void push(T value) {
		std::lock_guard lock(mutex_);
		if (closed_) return;
		if (queue_.size() == capacity_) queue_.pop_front();
		queue_.push_back(std::move(value));
		condition_.notify_one();
	}

	std::optional<T> wait_pop() {
		std::unique_lock lock(mutex_);
		condition_.wait(lock, [this] { return closed_ || !queue_.empty(); });
		return pop_locked();
	}

	std::optional<T> try_pop() {
		std::lock_guard lock(mutex_);
		return pop_locked();
	}

	void close() {
		std::lock_guard lock(mutex_);
		closed_ = true;
		condition_.notify_all();
	}

private:
	std::optional<T> pop_locked() {
		if (queue_.empty()) return std::nullopt;
		T value = std::move(queue_.front());
		queue_.pop_front();
		return value;
	}

	std::size_t capacity_;
	std::deque<T> queue_;
	std::mutex mutex_;
	std::condition_variable condition_;
	bool closed_{false};
};

} // namespace syspulse
