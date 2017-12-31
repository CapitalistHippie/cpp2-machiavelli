#ifndef MACHIAVELLI_MACH_INFRA_CONCURRENTAUTOINCREMENT_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_CONCURRENTAUTOINCREMENT_H_INCLUDED

#include <atomic>

namespace mach
{
namespace infra
{
template<typename T>
class ConcurrentAutoIncrement
{
  private:
    static std::atomic<T> autoIncrement;

    T increment;

  public:
    ConcurrentAutoIncrement()
      : increment(autoIncrement++)
    {
    }

    operator T() const noexcept
    {
        return increment;
    }

    bool operator==(T other) const noexcept
    {
        return increment == other;
    }
};

template<typename T>
std::atomic<T> ConcurrentAutoIncrement<T>::autoIncrement;
}
}

#endif // #ifndef MACHIAVELLI_MACH_INFRA_CONCURRENTAUTOINCREMENT_H_INCLUDED
