#include <thread/mutex.h>

using namespace lh::thread;

Mutex::Mutex() {}

Mutex::~Mutex() {}

void Mutex::lock() {
	__mutex.lock();
}

bool Mutex::tryLock() {
	return __mutex.try_lock();
}

void Mutex::unLock() {
	return __mutex.unlock();
}