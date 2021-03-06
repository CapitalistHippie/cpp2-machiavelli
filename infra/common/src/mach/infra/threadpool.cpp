#include "mach/infra/threadpool.h"

#include <iostream>

using namespace mach::infra;

void mach::infra::ThreadPool::InitializeThreads(unsigned int threadCount)
{
    shouldStop = false;

    while (threads.size() < threadCount)
    {
        threads.emplace_back(&ThreadPool::ThreadEntryPoint, this);
    }
}

void ThreadPool::ThreadEntryPoint()
{
    try
    {
        while (true)
        {
            if (shouldStop)
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
    catch (const std::exception& e)
    {
        std::cerr << "A fatal exception ocurred in a threadpool thread: " << e.what() << "\n";
    }
}

ThreadPool::ThreadPool(unsigned int threadCount)
  : shouldStop(false)
{
    InitializeThreads(threadCount);
}

void mach::infra::ThreadPool::ClearTasks()
{
    taskQueue.Clear();
}

void mach::infra::ThreadPool::StopThreads()
{
    shouldStop = true;

    ClearTasks();

    waitForNewTaskCondition.notify_all();

    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}
