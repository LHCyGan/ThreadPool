#include <thread/condition.h>

using namespace lh::thread;

Condition::Condition() {

}

Condition::~Condition() {}

void Condition::wait(Mutex* mutex) {
	// �������ȴ�ͬ���̻߳���
	std::unique_lock<std::mutex> lock(mutex->mutex_);
	__cond.wait(lock);
}

void Condition::signal() {
	// �������һ�������߳�
	__cond.notify_one();
}

void Condition::broadcast() {
	// ���������߳�
	__cond.notify_all();
}