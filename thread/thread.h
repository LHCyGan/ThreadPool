#pragma once

#include <thread>
#include <thread/task.h>
#include <thread/condition.h>
#include <thread/mutex.h>

namespace lh {
namespace thread {

	class Thread {

	public:
		Thread();
		virtual ~Thread();

		virtual void run() = 0;

		void start();
		void stop();

	protected:
		static void* thread_func(void* ptr);
	protected:
		Task* _task;
		Mutex _mutex;
		Condition _cond;
	};

}
}