#ifndef MACHIAVELLI_MACH_APP_EVENTS_EVENTBASE_H_INCLUDED
#define MACHIAVELLI_MACH_APP_EVENTS_EVENTBASE_H_INCLUDED

#include <string>

#include <mach/infra/abstractfactory.h>
#include <mach/infra/serializable.h>

#include "mach/app/events/event.h"
#include "mach/app/eventtype.h"
#include "mach/app/eventvisitor.h"

namespace mach
{
namespace app
{
namespace events
{
template<typename TDerived, EventType identifier>
class EventBase : public Event
{
  private:
    static const infra::AbstractFactory<Event, EventType>::ProductRegistrar<TDerived> registrar;
    static const infra::AbstractFactory<infra::Serializable, EventType>::ProductRegistrar<TDerived> registrar2;

  public:
    virtual ~EventBase() noexcept = default;

    std::string GetIdentifier() const override
    {
        registrar.Dummy();
        registrar2.Dummy();

        return std::to_string(static_cast<unsigned int>(identifier));
    }

    void Visit(const EventVisitor& visitor) const override
    {
        visitor.Visit(static_cast<const TDerived&>(*this));
    }
}; // class EventBase

template<typename TProduct, EventType identifier>
const infra::AbstractFactory<Event, EventType>::ProductRegistrar<TProduct> EventBase<TProduct, identifier>::registrar(
  identifier);

template<typename TProduct, EventType identifier>
const infra::AbstractFactory<infra::Serializable, EventType>::ProductRegistrar<TProduct>
  EventBase<TProduct, identifier>::registrar2(identifier);
} // namespace events
} // namespace app
} // namespace mach

#endif // MACHIAVELLI_MACH_APP_EVENTS_EVENTBASE_H_INCLUDED
