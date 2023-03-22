// 修复 windows 系统 min,max 宏冲突
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <iostream>
#include <windows.h> // windows sleep
//#include <unistd.h> // linux sleep

#include <util/logger.h>
#include <util/singleton.h>
using namespace lh::util;

// c++11 版本线程池
#include <thread/task.h>
#include <thread/task_dispatcher.h>
using namespace lh::thread;

#include <task/echo_task.h>
using namespace lh::task;

int main()
{
    // init logger
    Logger::instance()->open("./main.log");

    // init the task queue and thread pool
    int threads = 4;
    Singleton<TaskDispatcher>::instance()->init(threads);

    for (int i = 0; i < 10; i++)
    {
        int* num = new int;
        *num = i;
        Task* task = new EchoTask(num);
        Singleton<TaskDispatcher>::instance()->assign(task);
    }

    while (!Singleton<TaskDispatcher>::instance()->empty())
    {
        Sleep(1000); // windows sleep
        //        sleep(1); // linux sleep
    }
    return 0;
}
