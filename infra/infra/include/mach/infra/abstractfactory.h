#ifndef MACHIAVELLI_MACH_INFRA_ABSTRACTFACTORY_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_ABSTRACTFACTORY_H_INCLUDED

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "mach/infra/noncopyable.h"

namespace mach
{
namespace infra
{
namespace detail
{
// Stolen from http://en.cppreference.com/w/cpp/utility/make_from_tuple
template<class T, class Tuple, std::size_t... I>
constexpr T make_from_tuple_impl(Tuple&& t, std::index_sequence<I...>)
{
    return T(std::get<I>(std::forward<Tuple>(t))...);
}

template<class T, class Tuple>
constexpr T make_from_tuple(Tuple&& t)
{
    return detail::make_from_tuple_impl<T>(
      std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

class ConstructionArguments
{
  public:
    virtual ~ConstructionArguments() = default;
};

template<typename... TArgs>
class ConstructionArgumentsImpl : public ConstructionArguments
{
  public:
    std::tuple<TArgs&&...> arguments;

    ConstructionArgumentsImpl(TArgs&&... args)
      : arguments(std::forward_as_tuple<TArgs&&...>(args...))
    {
    }
};

class ProductConstructor
{
  public:
    virtual ~ProductConstructor() = default;

    virtual std::shared_ptr<void> Construct(std::shared_ptr<ConstructionArguments> arguments) const = 0;
};

template<typename TProduct, typename... TArgs>
class ProductConstructorImpl : public ProductConstructor
{
  public:
    std::shared_ptr<void> Construct(std::shared_ptr<ConstructionArguments> arguments) const override
    {
        auto castedArguments = std::static_pointer_cast<ConstructionArgumentsImpl<TArgs...>>(arguments);

        auto product = make_from_tuple<TProduct>(castedArguments->arguments);

        return std::make_shared<TProduct>(std::move(product));
    }
};
} // namespace detail

template<typename TBase, typename TIdentifier>
class AbstractFactory : public infra::Noncopyable
{
  private:
    std::unordered_map<TIdentifier, std::shared_ptr<detail::ProductConstructor>> productConstructors;

  public:
    template<typename TProduct, typename... TArgs>
    class ProductRegistrar
    {
      public:
        ProductRegistrar(TIdentifier identifier)
        {
            auto& factory = AbstractFactory<TBase, TIdentifier>::GetInstance();

            factory.RegisterProduct<TProduct, TArgs...>(identifier);
        }

        int Dummy() const noexcept
        {
            return 0;
        }
    };

    static AbstractFactory& GetInstance() noexcept
    {
        static AbstractFactory factory;
        return factory;
    }

    template<typename... TArgs>
    std::shared_ptr<TBase> Construct(const TIdentifier& identifier, TArgs&&... args) const
    {
        return std::static_pointer_cast<TBase>(
          productConstructors.at(identifier)
            ->Construct(std::make_shared<detail::ConstructionArgumentsImpl<TArgs...>>(std::forward<TArgs>(args)...)));
    }

    template<typename TProduct, typename... TArgs>
    void RegisterProduct(TIdentifier identifier)
    {
        productConstructors.emplace(identifier, std::make_shared<detail::ProductConstructorImpl<TProduct, TArgs...>>());
    }
}; // class AbstractFactory
} // namespace infra
} // namespace mach

#endif // MACHIAVELLI_MACH_INFRA_ABSTRACTFACTORY_H_INCLUDED
