#include "mach/infra/threadpool.h"

using namespace mach::infra;

void mach::infra::ThreadPool::InitializeThreads(unsigned int threadCount)
{
    while (threads.size() < threadCount)
    {
        threads.emplace_back(&ThreadPool::ThreadEntryPoint, this);
    }
}

void ThreadPool::ThreadEntryPoint()
{
    while (true)
    {
        if (shouldTerminate)
        {
            break;
        }

        std::shared_ptr<detail::Task> taskBuffer;

        if (taskQueue.Pop(taskBuffer))
        {
            taskBuffer->Run();
        }
        else
        {
            std::unique_lock<std::mutex> conditionLock(waitForNewtaskConditionMutex);
            waitForNewTaskCondition.wait(conditionLock);
        }
    }
}

ThreadPool::ThreadPool(unsigned int threadCount)
  : shouldTerminate(false)
{
    InitializeThreads(threadCount);
}

ThreadPool::~ThreadPool() noexcept
{
    taskQueue.Clear();

    shouldTerminate = true;

    waitForNewTaskCondition.notify_all();

    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}
