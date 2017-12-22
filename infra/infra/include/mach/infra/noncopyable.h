#ifndef MACHIAVELLI_MACH_INFRA_NONCOPYABLE_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_NONCOPYABLE_H_INCLUDED

namespace mach
{
namespace infra
{
class Noncopyable
{
  private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;

  public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(Noncopyable&& other) = default;
    Noncopyable& operator=(Noncopyable&& other) = default;
}; // class Noncopyable
} // namespace infra
} // namespace mach

#endif // MACHIAVELLI_MACH_INFRA_NONCOPYABLE_H_INCLUDED
