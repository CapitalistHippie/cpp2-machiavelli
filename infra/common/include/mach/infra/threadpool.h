#ifndef MACHIAVELLI_MACH_INFRA_THREADPOOL_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_THREADPOOL_H_INCLUDED

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include "mach/infra/concurrentqueue.h"
#include "mach/infra/noncopyable.h"

namespace mach
{
namespace infra
{
namespace detail
{
class Task
{
  public:
    virtual void Run() const = 0;
}; // class Task

template<typename TTask>
class TaskImpl : public Task
{
  private:
    TTask task;

  public:
    TaskImpl(TTask task)
      : task(task)
    {
    }

    void Run() const override
    {
        task();
    }
}; // class TaskImpl
} // namespace detail

class ThreadPool : public Noncopyable
{
  private:
    std::condition_variable waitForNewTaskCondition;
    std::mutex waitForNewtaskConditionMutex;

    std::vector<std::thread> threads;
    ConcurrentQueue<std::shared_ptr<detail::Task>> taskQueue;

    std::atomic<bool> shouldStop;

    void InitializeThreads(unsigned int threadCount);

    void ThreadEntryPoint();

  public:
    ThreadPool(unsigned int threadCount);
    ThreadPool(ThreadPool&&) = default;
    ThreadPool& operator=(ThreadPool&&) = default;

    template<typename TTask>
    void QueueTask(TTask task)
    {
        if (shouldStop)
        {
            return;
        }

        taskQueue.Push(std::make_shared<detail::TaskImpl<TTask>>(task));

        waitForNewTaskCondition.notify_one();
    }

    void ClearTasks();
    void StopThreads();
}; // class ThreadPool
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_THREADPOOL_H_INCLUDED
