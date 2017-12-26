#ifndef MACHIAVELLI_MACH_INFRA_CONCURRENTQUEUE_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_CONCURRENTQUEUE_H_INCLUDED

#include <mutex>
#include <queue>

namespace mach
{
namespace infra
{
template<typename T>
class ConcurrentQueue
{
  private:
    std::mutex queueMutex;

    std::queue<T> queue;

  public:
    void Push(T item)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        queue.push(item);
    }

    T Pop()
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        auto item = queue.front();
        queue.pop();

        return item;
    }

    bool Pop(T& itemBuffer)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (queue.empty())
        {
            return false;
        }

        itemBuffer = queue.front();
        queue.pop();

        return true;
    }

    bool IsEmpty()
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        return queue.empty();
    }
}; // class ConcurrentQueue
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_CONCURRENTQUEUE_H_INCLUDED
