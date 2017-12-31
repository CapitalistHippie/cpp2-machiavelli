#ifndef MACHIAVELLI_MACH_INFRA_COMMANDMEDIATOR_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_COMMANDMEDIATOR_H_INCLUDED

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "mach/infra/concurrentautoincrement.h"
#include "mach/infra/observable.h"

namespace mach
{
namespace infra
{
namespace detail
{
class ObservablePredicate
{
  public:
    virtual ~ObservablePredicate() noexcept = default;
    virtual bool Predicate(const Observable& command) const = 0;
}; // class ObservablePredicate

template<typename TObservable, typename TPredicate>
class ObservablePredicateImpl : public ObservablePredicate
{
  private:
    TPredicate predicate;

  public:
    ObservablePredicateImpl(TPredicate predicate)
      : predicate(predicate)
    {
    }

    bool Predicate(const Observable& command) const override
    {
        return predicate(static_cast<const TObservable&>(command));
    }
}; // class ObservablePredicateImpl

class Observer
{
  public:
    virtual ~Observer() noexcept = default;
    virtual bool CanHandleType(std::type_index type) const = 0;
    virtual void Notify(const Observable& command) const = 0;
}; // class Observer

template<typename TObservable, typename TObserver>
class ObserverImpl : public Observer
{
  private:
    TObserver observer;

  public:
    typedef unsigned int Id;

    infra::ConcurrentAutoIncrement<Id> id;

    ObserverImpl(TObserver observer)
      : observer(observer)
    {
    }

    bool CanHandleType(std::type_index type) const override
    {
        return type == typeid(TObservable);
    }

    void Notify(const Observable& command) const override
    {
        return observer(static_cast<const TObservable&>(command));
    }
}; // class ObserverImpl
} // namespace detail

class Subject
{
  public:
    typedef unsigned int ObserverHandle;

  private:
    std::unordered_map<int, std::pair<std::shared_ptr<detail::ObservablePredicate>, std::shared_ptr<detail::Observer>>>
      predicateObservers;

    std::unordered_map<int, std::shared_ptr<detail::Observer>> observers;

  public:
    template<typename TObservable>
    void NotifyObservers(const TObservable& observable) const
    {
        // We split the finding of observers and notifying them so if the observer unregisters an observer the container
        // isn't modified while we're iterating over it.
        std::vector<std::shared_ptr<detail::Observer>> observersToNotify;

        for (auto predicateHandlerMapPair : predicateObservers)
        {
            const auto& predicateHandler = predicateHandlerMapPair.second;

            if (predicateHandler.second->CanHandleType(typeid(TObservable)) &&
                predicateHandler.first->Predicate(static_cast<const Observable&>(observable)))
            {
                observersToNotify.push_back(predicateHandler.second);
            }
        }

        for (auto observer : observers)
        {
            if (observer.second->CanHandleType(typeid(TObservable)))
            {
                observersToNotify.push_back(observer.second);
            }
        }

        for (auto observer : observersToNotify)
        {
            observer->Notify(observable);
        }
    }

    template<typename TObservable, typename TPredicate, typename TObserver>
    ObserverHandle RegisterObserver(TPredicate predicate, TObserver observer)
    {
        auto observerObj = std::make_shared<detail::ObserverImpl<TObservable, TObserver>>(observer);

        predicateObservers.emplace(
          observerObj->id,
          std::make_pair(std::make_shared<detail::ObservablePredicateImpl<TObservable, TPredicate>>(predicate),
                         observerObj));

        return observerObj->id;
    }

    template<typename TObservable, typename TObserver>
    ObserverHandle RegisterObserver(TObserver observer)
    {
        auto observerObj = std::make_shared<detail::ObserverImpl<TObservable, TObserver>>(observer);

        observers.emplace(observerObj->id, observerObj);

        return observerObj->id;
    }

    void UnregisterPredicateObserver(ObserverHandle handle);
    void UnregisterObserver(ObserverHandle handle);

    void Clear();
}; // class Subject
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_COMMANDMEDIATOR_H_INCLUDED
