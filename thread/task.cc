#include <thread/task.h>

using namespace lh::thread;

Task::Task() : _data(nullptr) {}

Task::Task(void* data) : _data(data) {}

Task::~Task() {

}

void* Task::getData() {
	return _data;
}

void Task::setData(void* data) {
	_data = data;
}
