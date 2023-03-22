#pragma once

#include <condition_variable>
#include <thread/mutex.h>

namespace lh {
namespace thread {

	class Condition {
	public:
		Condition();
		~Condition();

		void wait(Mutex* mutex);
		void signal();
		void broadcast();

	protected:
		std::condition_variable __cond;
	};

}
}