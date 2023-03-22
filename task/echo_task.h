#pragma once

#include <thread/task.h>

using namespace lh::thread;

namespace lh {
namespace task {

	class EchoTask : public Task {
	public:
		EchoTask(int* num) : Task((void*)num) {}
		virtual ~EchoTask() {}

		virtual void run() {
			printf("task run\n");
			int* input = static_cast<int*>(_data);
			printf("num: %d\n", *input);
		}

		virtual void destroy() {
			printf("task destroy\n");
			delete static_cast<int*>(_data);
			delete this;
		}
	};
}
}