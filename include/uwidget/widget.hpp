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

    static_assert((detail::is_policy_v<Policies> && ...), "all types in Policies must inherit from policy_base");

    /**
     * @brief Helper member to indicate if this widget can be moved
     */
    inline static constexpr bool can_move =
      !w_has_policy_v<policy::NoMove> &&
      std::is_move_constructible_v<value_t> &&
      std::is_move_assignable_v<value_t>;

    /**
     * @brief Helper member to indicate if this widget can be copied
     */
    inline static constexpr bool can_copy =
      !w_has_policy_v<policy::NoCopy> &&
      std::is_copy_constructible_v<value_t> &&
      std::is_copy_assignable_v<value_t>;

    /**
     * @brief Helper member to indicate if this widget can be default constructed
     */
    inline static constexpr bool can_default_construct =
      !w_has_policy_v<policy::NoDefaultConstruct> &&
      std::is_default_constructible_v<value_t>;

    /**
     * @brief Helper method to get this specific Widget's session type.
     */
    auto& get_session() {
      return Session<Widget>::instance();
    }

    /**
     * @brief Default constructor. Default constructs value and records this in the session.
     * ThrowOnDefaultConstruction widgets will throw without session tracking.
     */
    Widget() requires(can_default_construct) {
      if constexpr(w_has_policy_v<policy::ThrowOnDefaultConstruction>) {
        throw WidgetException("uwidget: ThrowOnDefaultconstruction");
      }
      get_session().template reg<RegisteredData::DefaultConstructions>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }

    /**
     * @brief lvalue Value construction. Copy constructs value and records this in the session.
     */
    explicit Widget(const value_t& x) : value{x} {
      get_session().template reg<RegisteredData::ValueConstruction>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }

    /**
     * @brief rvalue Value construction. Move constructs value and records this in the session.
     */
    explicit Widget(value_t&& x) : value{std::move(x)} {
      get_session().template reg<RegisteredData::ValueConstruction>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }

    /**
     * @brief Destructor. Records this in the session.
     */
    ~Widget() {
      get_session().template reg<RegisteredData::Destructions>();
      get_session().template dereg<RegisteredData::ActiveInstances>();
    }

    /**
     * @brief Move constructor. Move constructs value and records this in the session.
     * ThrowOnMove widgets will throw before moving from other's value, without session tracking.
     * Move construction is noexcept if the widget is not ThrowOnMove and value_t is nothrow move constructible.
     */
    Widget(Widget&& other)
      noexcept(
          !w_has_policy_v<policy::ThrowOnMove> &&
          std::is_nothrow_move_constructible_v<value_t>
      )
      requires(can_move) :
      value{w_has_policy_v<policy::ThrowOnMove> ? throw WidgetException("uwidget: ThrowOnMove")
                                                : std::move(other.value)}
    {
      get_session().template reg<RegisteredData::MoveConstructions>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }

    /**
     * @brief Explicitly deleted move constructor if the Widget is policy-constrained.
     * This is required to prevent NoMove widgets from falling back on copy operations.
     */
    Widget(Widget&& other)  = delete;

    /**
     * @brief Move assignment operator . Move assigns value and records this in the session.
     * ThrowOnMove widgets will throw before moving from other's value, without session tracking.
     * Move assignment is noexcept if the widget is not ThrowOnMove and value_t is nothrow move assignable.
     */
    Widget& operator=(Widget&& other)
      noexcept(
          !w_has_policy_v<policy::ThrowOnMove> &&
          std::is_nothrow_move_assignable_v<value_t>
      )
      requires(can_move) {
      if constexpr(w_has_policy_v<policy::ThrowOnMove>) {
        throw WidgetException("uwidget: ThrowOnMove");
      }
      get_session().template reg<RegisteredData::MoveAssignments>();
      value = std::move(other.value);
      return *this;
    }

    /**
     * @brief Explicitly deleted move assignment operator if the Widget is policy-constrained.
     * This is required to prevent NoMove widgets from falling back on copy operations.
     */
    Widget& operator=(Widget&& other) = delete;

    /**
     * @brief Copy constructor. Copy constructs value and records this in the session.
     * ThrowOnCopy widgets will throw before constructor is entered, without session tracking.
     */
    Widget(const Widget& other) requires(can_copy) :
      value{w_has_policy_v<policy::ThrowOnCopy> ? throw WidgetException("uwidget: ThrowOnCopy")
                                                : other.value}
    {
      get_session().template reg<RegisteredData::CopyConstructions>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }

    /**
     * @brief Copy assignment operator. Copy assigns value and records this in the session.
     * ThrowOnCopy widgets will throw before value is assigned, without session tracking.
     */
    Widget& operator=(const Widget& other) requires(can_copy) {
      if constexpr(w_has_policy_v<policy::ThrowOnCopy>) {
        throw WidgetException("uwidget: ThrowOnCopy");
      }
      get_session().template reg<RegisteredData::CopyAssignments>();
      value = other.value;
      return *this;
    }

  };

}
