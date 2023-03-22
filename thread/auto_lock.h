#pragma once

#include <thread/mutex.h>

namespace lh {
namespace thread {

	class AutoLock {
	public:
		AutoLock(Mutex* mutex);
		~AutoLock();

	private:
		Mutex* __mutex;
	};

}
}