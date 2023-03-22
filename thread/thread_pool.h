#pragma once

#include <list>

#include <thread/thread.h>
#include <thread/mutex.h>
#include <thread/auto_lock.h>
#include <thread/auto_lock.h>
#include <thread/task.h>

namespace lh {
namespace thread {

	class ThreadPool {
	public:
		ThreadPool();
		~ThreadPool();

		void create(int threads);

		Thread* get();
		void put(Thread* thread);
		bool empty();
		void assign(Task* task);

	private:
		int _threads;
		std::list<Thread*> _pool;
		Mutex _mutex;
		Condition _cond;
	};
}
}