#pragma once

#include <mutex>

namespace lh {
namespace thread {

    class Mutex
    {
        friend class Condition;

    public:
        Mutex();
        ~Mutex();

        // lock
        void lock();

        // try to lock
        bool tryLock();

        // unlock
        void unLock();

    private:
        std::mutex __mutex;
    };

}
}
