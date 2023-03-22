#include <thread/condition.h>

using namespace lh::thread;

Condition::Condition() {

}

Condition::~Condition() {}

void Condition::wait(Mutex* mutex) {
	// 阻塞，等待同步线程唤醒
	std::unique_lock<std::mutex> lock(mutex->mutex_);
	__cond.wait(lock);
}

void Condition::signal() {
	// 随机唤醒一个阻塞线程
	__cond.notify_one();
}

void Condition::broadcast() {
	// 唤醒所有线程
	__cond.notify_all();
}