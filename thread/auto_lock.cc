#include <thread/auto_lock.h>

using namespace lh::thread;

AutoLock::AutoLock(Mutex* mutex) {
	__mutex = mutex;
	__mutex->lock();
}

AutoLock::~AutoLock() {
	__mutex->unLock();
}

