#include <thread/work_thread.h>
#include <thread_pool.h>
#include <util/logger.h>
#include <util/singleton.h>

using namespace lh::thread;
using namespace lh::util;

WorkerThread::WorkerThread() : Thread() {

}

WorkerThread::~WorkerThread() {

}

void WorkerThread::run() {
	while (true) {
		// 无task时阻塞当前工作线程
		while (_task == nullptr)
			_cond.wait(&_mutex);

		log_debug("worker thread run: thread=%x, task=%x", this, _task);
		_task->run();
		_task->destroy();
		_task = nullptr;
		// 有task时，将任务加入线程池
		Singleton<ThreadPool>::instance()->put(this);
	}
}