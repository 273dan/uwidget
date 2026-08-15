#pragma once

#include <concepts>
#include <type_traits>
#include <compare>
#include "policy.hpp"
#include "detail.hpp"
#include "widget_exception.hpp"

namespace uwidget {
  
  template <typename ...Policies>
  class Widget {
  public:

    inline static thread_local size_t active_instances = 0;
    inline static thread_local size_t destructions = 0;
    inline static thread_local size_t default_constructions = 0;
    inline static thread_local size_t copy_constructions = 0;
    inline static thread_local size_t copy_assignments = 0;
    inline static thread_local size_t move_constructions = 0;
    inline static thread_local size_t move_assignments = 0;
    inline static thread_local size_t value_constructions = 0;

    template <typename TargetT>
    inline static constexpr bool w_has_policy_v = detail::has_policy_v<TargetT, Policies...>;

    using value_t = detail::get_value_type_t<int, Policies...>;
    value_t value;

    static_assert((detail::is_policy_v<Policies> && ...), "all types in Policies must inherit from policy_base");
    static_assert(!detail::contains_multiple_value_policies_v<Policies...>, "Widget must contain at most 1 Value policy");

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
     * @brief Reset all tracked metric counters to 0 for this Widget type.
     */
    static void reset_metrics() {
      active_instances = 0;
      destructions = 0;
      default_constructions = 0;
      copy_constructions = 0;
      copy_assignments = 0;
      move_constructions = 0;
      move_assignments = 0;
      value_constructions = 0;
    }

    /**
     * @brief Default constructor. Default constructs value and records this in the session.
     * ThrowOnDefaultConstruction widgets will throw without session tracking.
     */
    Widget() requires(can_default_construct) {
      if constexpr(w_has_policy_v<policy::ThrowOnDefaultConstruction>) {
        throw WidgetException("uwidget: ThrowOnDefaultconstruction");
      }
      default_constructions++;
      active_instances++;
    }

    /**
     * @brief lvalue Value construction. Copy constructs value and records this in the session.
     */
    explicit Widget(const value_t& x) : value{x} {
      value_constructions++;
      active_instances++;
    }

    /**
     * @brief rvalue Value construction. Move constructs value and records this in the session.
     */
    explicit Widget(value_t&& x) : value{std::move(x)} {
      value_constructions++;
      active_instances++;
    }

    /**
     * @brief Destructor. Records this in the session.
     */
    ~Widget() {
      destructions++;
      active_instances--;
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
      move_constructions++;
      active_instances++;
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
      move_assignments++;
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
      copy_constructions++;
      active_instances++;
    }

    /**
     * @brief Copy assignment operator. Copy assigns value and records this in the session.
     * ThrowOnCopy widgets will throw before value is assigned, without session tracking.
     */
    Widget& operator=(const Widget& other) requires(can_copy) {
      if constexpr(w_has_policy_v<policy::ThrowOnCopy>) {
        throw WidgetException("uwidget: ThrowOnCopy");
      }
      copy_assignments++;
      value = other.value;
      return *this;
    }


  };

  /**
   * @brief Spaceship operator. Compares Widget.value.
   * Requires value_t to be three way comparable
   */
  template <typename ...Policies>
  constexpr auto operator <=>(const Widget<Policies...>& l, const Widget<Policies...>& r)
    requires(std::three_way_comparable<typename Widget<Policies...>::value_t>) {
      return l.value <=> r.value;
    }

  /**
   * @brief Equality comparison operator. Compares value.
   * Requires value_t to be equality comparable
   */
  template <typename ...Policies>
  constexpr bool operator==(const Widget<Policies...>& l, const Widget<Policies...>& r)
    requires(std::equality_comparable<typename Widget<Policies...>::value_t>) {
      return l.value == r.value;
    }

}

namespace std {
  template <typename ...Policies>
    struct hash<uwidget::Widget<Policies...>> {
      size_t operator()(const uwidget::Widget<Policies...>& w) const noexcept {
        using V = typename uwidget::Widget<Policies...>::value_t;
        return std::hash<V>{}(w.value);
      }
    };
}
