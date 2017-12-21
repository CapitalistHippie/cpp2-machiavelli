#ifndef MACHIAVELLI_MACH_INFRA_COMMANDMEDIATOR_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_COMMANDMEDIATOR_H_INCLUDED

#include <functional>
#include <memory>
#include <typeindex>
#include <utility>
#include <vector>

#include "mach/infra/command.h"

namespace mach
{
namespace infra
{
namespace detail
{
class CommandPredicate
{
  public:
    virtual ~CommandPredicate() noexcept = default;
    virtual bool CanHandleType(std::type_index type) const = 0;
    virtual bool Predicate(const Command& command) const = 0;
}; // class CommandPredicate

template<typename TCommand, typename TPredicate>
class CommandPredicateImpl : public CommandPredicate
{
  private:
    TPredicate predicate;

  public:
    CommandPredicateImpl(TPredicate predicate)
      : predicate(predicate)
    {
    }

    bool CanHandleType(std::type_index type) const override { return type == typeid(TCommand); }

    bool Predicate(const Command& command) const override { return predicate(static_cast<const TCommand&>(command)); }
}; // class CommandPredicateImpl

class CommandHandler
{
  public:
    virtual ~CommandHandler() noexcept = default;
    virtual void Handle(const Command& command) const = 0;
}; // class CommandHandler

template<typename TCommand, typename THandler>
class CommandHandlerImpl : public CommandHandler
{
  private:
    THandler handler;

  public:
    CommandHandlerImpl(THandler handler)
      : handler(handler)
    {
    }

    void Handle(const Command& command) const override { return handler(static_cast<const TCommand&>(command)); }
}; // class CommandHandlerImpl
} // namespace detail

class CommandMediator
{
  private:
    std::vector<std::pair<std::shared_ptr<detail::CommandPredicate>, std::shared_ptr<detail::CommandHandler>>>
      predicateHandlers;

  public:
    template<typename TCommand>
    void HandleCommand(const TCommand& command) const
    {
        for (auto predicateHandler : predicateHandlers)
        {
            if (predicateHandler.first->CanHandleType(typeid(TCommand)) && predicateHandler.first->Predicate(command))
            {
                predicateHandler.second->Handle(command);
            }
        }
    }

    template<typename TCommand, typename TPredicate, typename THandler>
    void RegisterCommandHandler(TPredicate predicate, THandler handler)
    {
        predicateHandlers.push_back(
          std::make_pair(std::make_shared<detail::CommandPredicateImpl<TCommand, TPredicate>>(predicate),
                         std::make_shared<detail::CommandHandlerImpl<TCommand, THandler>>(handler)));
    }

    void Clear();
}; // class CommandMediator
} // namespace infra
} // namespace mach

#endif // MACHIAVELLI_MACH_INFRA_COMMANDMEDIATOR_H_INCLUDED
