#pragma once

#include <thread/thread.h>

namespace lh {
namespace thread {

	class WorkerThread : public Thread {
	public:
		WorkerThread();
		virtual ~WorkerThread();

		virtual void run();
	};
}
}