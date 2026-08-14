#pragma once

#include <type_traits>
#include "policy.hpp"
#include "session.hpp"
#include "detail.hpp"
#include "widget_exception.hpp"

namespace uwidget {
  
  //                 a single policied-widget will have that policy interpreted as "ValueT", so 
  //                 we call the first parameter FirstOrValueT
  //                 v
  template <typename FirstOrValueT = int, typename ...Policies>
  class Widget {
  public:
    template <typename TargetT>
    inline static constexpr bool w_has_policy_v = detail::has_policy_v<TargetT, FirstOrValueT, Policies...>;

    using value_t = std::conditional_t<detail::is_policy_v<FirstOrValueT>, int, FirstOrValueT>;
    value_t value;

    inline static constexpr bool can_move =
      !w_has_policy_v<policy::NoMove> &&
      std::is_move_constructible_v<value_t> &&
      std::is_move_assignable_v<value_t>;

    inline static constexpr bool can_copy =
      !w_has_policy_v<policy::NoCopy> &&
      std::is_copy_constructible_v<value_t> &&
      std::is_copy_assignable_v<value_t>;

    inline static constexpr bool can_default_construct =
      !w_has_policy_v<policy::NoDefaultConstruct> &&
      std::is_default_constructible_v<value_t>;

    auto& get_session() {
      return Session<Widget>::instance();
    }

    // default construction
    Widget() requires(can_default_construct) {
      if constexpr(w_has_policy_v<policy::ThrowOnDefaultConstruction>) {
        throw WidgetException("uwidget: ThrowOnDefaultconstruction");
      }
      get_session().template reg<RegisteredData::DefaultConstructions>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }
    Widget() requires(!can_default_construct) = delete;

    // value construction
    explicit Widget(const value_t& x) : value{x} {
      get_session().template reg<RegisteredData::ValueConstruction>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }

    explicit Widget(value_t&& x) : value{std::move(x)} {
      get_session().template reg<RegisteredData::ValueConstruction>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }

    // destruction
    ~Widget() {
      get_session().template reg<RegisteredData::Destructions>();
      get_session().template dereg<RegisteredData::ActiveInstances>();
    }

    // move construction
    Widget(Widget&& other) noexcept(!w_has_policy_v<detail::ThrowOnMove>) requires(can_move) : value{std::move(other.value)} {
      if constexpr(w_has_policy_v<policy::ThrowOnMove>) {
        throw WidgetException("uwidget: ThrowOnMove");
      }
      get_session().template reg<RegisteredData::MoveConstructions>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }
    Widget(Widget&&) requires(!can_move) = delete;

    // move assignment
    Widget& operator=(Widget&& other) noexcept(!w_has_policy_v<detail::ThrowOnMove>) requires(can_move) {
      if constexpr(w_has_policy_v<policy::ThrowOnMove>) {
        throw WidgetException("uwidget: ThrowOnMove");
      }
      get_session().template reg<RegisteredData::MoveAssignments>();
      value = std::move(other.value);
      return *this;
    }
    Widget& operator=(Widget&&) requires(!can_move) = delete;

    // copy construction
    Widget(const Widget& other) requires(can_copy) : value{other.value} {
      if constexpr(w_has_policy_v<policy::ThrowOnCopy>) {
        throw WidgetException("uwidget: ThrowOnCopy");
      }
      get_session().template reg<RegisteredData::CopyConstructions>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }
    Widget(const Widget&) requires(!can_copy) = delete;

    // copy assignment
    Widget& operator=(const Widget& other) requires(can_copy) {
      if constexpr(w_has_policy_v<policy::ThrowOnCopy>) {
        throw WidgetException("uwidget: ThrowOnCopy");
      }
      get_session().template reg<RegisteredData::CopyAssignments>();
      value = other.value;
      return *this;
    }
    Widget& operator=(const Widget&) requires(!can_copy) = delete;

  };

}
