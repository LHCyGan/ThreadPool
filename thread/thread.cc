#include <thread/thread.h>
#include <thread/auto_lock.h>

using namespace lh::thread;

Thread::Thread{
	_task(nullptr);
}


Thread::~Thread() {

}

void Thread::start() {
	std::thread t(Thread::thread_func, this);
	t.detach();
}

void Thread::stop() {

}

void* Thread::thread_func(void* ptr) {
	auto thread = (Thread*)ptr;
	thread->run();

	return ptr;
}

void Thread::set_task(Task* task) {
	AutoLock lock(_mutex);
	_task = task;
	_cond.signal();
}

Task* Thread::get_task() {
	AutoLock lock(_mutex);

	return _task;
}