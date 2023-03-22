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
		// ��taskʱ������ǰ�����߳�
		while (_task == nullptr)
			_cond.wait(&_mutex);

		log_debug("worker thread run: thread=%x, task=%x", this, _task);
		_task->run();
		_task->destroy();
		_task = nullptr;
		// ��taskʱ������������̳߳�
		Singleton<ThreadPool>::instance()->put(this);
	}
}