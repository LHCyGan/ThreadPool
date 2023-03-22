#include <thread/task_dispatcher.h>
using namespace lh::thread;

#include <util/logger.h>
#include <util/singleton.h>
using namespace lh::util;

TaskDispatcher::TaskDispatcher() {

}

TaskDispatcher::~TaskDispatcher() {

}

void TaskDispatcher::init(int threads) {
	Singleton<ThreadPool>::instance()->create(threads);
	start();
}

void TaskDispatcher::assign(Task* task) {
	log_debug("task dispatcher assign task: %x", task);
	AutoLock lock(&_mutex);
	_tasks.push_back(task);
	_cond.signal();
}

void TaskDispatcher::handle(Task* task) {
	ThreadPool* pool = Singleton<ThreadPool>::instance();
	if (!pool->empty()) {
		pool->assign(task);
	}
	else {
		AutoLock lock(&_mutex);
		_tasks.push_front(task);
	}
}

bool TaskDispatcher::empty() {
	AutoLock lock(&_mutex);
	return _tasks.empty()
}

void TaskDispatcher::run() {
	while (true) {
		while (_tasks.empty())
			_cond.wait(&_mutex);

		_mutex.lock();
		Task* task = _tasks.front();
		_tasks.pop_front();
		_mutex.unLock();
		handle(task);
	}
}