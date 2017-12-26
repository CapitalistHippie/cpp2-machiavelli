#ifndef MACHIAVELLI_MACH_INFRA_COMMANDMEDIATOR_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_COMMANDMEDIATOR_H_INCLUDED

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
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

    bool CanHandleType(std::type_index type) const override
    {
        return type == typeid(TCommand);
    }

    bool Predicate(const Command& command) const override
    {
        return predicate(static_cast<const TCommand&>(command));
    }
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

    void Handle(const Command& command) const override
    {
        return handler(static_cast<const TCommand&>(command));
    }
}; // class CommandHandlerImpl
} // namespace detail

class CommandMediator
{
  private:
    std::unordered_map<int,
                       std::pair<std::shared_ptr<detail::CommandPredicate>, std::shared_ptr<detail::CommandHandler>>>
      predicateHandlers;

    int index;

  public:
    typedef int CommandHandlerHandle;

    CommandMediator()
      : index(0)
    {
    }

    template<typename TCommand>
    void HandleCommand(const TCommand& command) const
    {
        // We split the finding of handlers and running them so if the handler unregisters a handler the container isn't
        // modified while we're iterating over it.
        std::vector<std::shared_ptr<detail::CommandHandler>> handlersToRun;

        for (auto predicateHandlerMapPair : predicateHandlers)
        {
            const auto& predicateHandler = predicateHandlerMapPair.second;

            if (predicateHandler.first->CanHandleType(typeid(TCommand)) && predicateHandler.first->Predicate(command))
            {
                handlersToRun.push_back(predicateHandler.second);
            }
        }

        for (auto handler : handlersToRun)
        {
            handler->Handle(command);
        }
    }

    template<typename TCommand, typename TPredicate, typename THandler>
    CommandHandlerHandle RegisterCommandHandler(TPredicate predicate, THandler handler)
    {
        predicateHandlers.emplace(
          index,
          std::make_pair(std::make_shared<detail::CommandPredicateImpl<TCommand, TPredicate>>(predicate),
                         std::make_shared<detail::CommandHandlerImpl<TCommand, THandler>>(handler)));

        return index++;
    }

    void UnregisterCommandHandler(CommandHandlerHandle handle)
    {
        predicateHandlers.erase(handle);
    }

    void Clear();
}; // class CommandMediator
} // namespace infra
} // namespace mach

#endif // MACHIAVELLI_MACH_INFRA_COMMANDMEDIATOR_H_INCLUDED
