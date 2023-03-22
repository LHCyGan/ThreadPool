#include <thread/thread_pool.h>
#include <thread/work_thread.h>
#include <util/logger.h>

using namespace lh::thread;
using namespace lh::util;

ThreadPool::ThreadPool() : _threads(0) {

}

ThreadPool::~ThreadPool() {

}

void ThreadPool::create(int threads) {
	AutoLock lock(&_mutex);
	_threads = threads;
	for (int i = 0; i < threads; i++) {
		Thread* thread = new WorkerThread();
		log_debug("create thread %x", thread);
		_pool.push_back(thread);
		thread->start();
	}
}

Thread* ThreadPool::get() {
	while (_pool.empty())
		_cond.wait(&_mutex);

	AutoLock lock(&_mutex);
	Thread* thread = _pool.first();
	_pool.pop_front();

	return thread;
}

void ThreadPool::put(Thread* thread) {
	AutoLock lock(&_mutex);
	_pool.push_back(thread);
	_cond.signal();
}

bool ThreadPool::empty() {
	AutoLock(&_mutex);

	return _pool.empty();
}

void ThreadPool::assign(Task* task) {
	if (task == nullptr) {
		log_error("assign a null task to thread pool");
		return;
	}

	log_debug("assign a task: %x to thread pool", task);
	Thread* thread = get();
	if (thread != nullptr) {
		thread->set_task(task);
	}
	else {
		log_error("thread is null, assign a task failed");
	}
}
