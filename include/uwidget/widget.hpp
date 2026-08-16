#pragma once

#include <concepts>
#include <format>
#include <type_traits>
#include <compare>
#include <array>
#include "policy.hpp"
#include "detail.hpp"
#include "widget_exception.hpp"
#include "operation.hpp"

namespace uwidget {
  
  template <typename ...Policies>
  class Widget {
  public:

    template <Op op>
    static size_t& get_metric() {
      return metrics_[static_cast<uint8_t>(op)];
    }

    static size_t active_instances() { return get_metric<Op::ActiveInstance>(); }
    static size_t destructions() { return get_metric<Op::Destruction>(); }
    static size_t copy_constructions() { return get_metric<Op::CopyConstruction>(); }
    static size_t move_constructions() { return get_metric<Op::MoveConstruction>(); }
    static size_t copy_assignments() { return get_metric<Op::CopyAssignment>(); }
    static size_t move_assignments() { return get_metric<Op::MoveAssignment>(); }
    static size_t default_constructions() { return get_metric<Op::DefaultConstruction>(); }
    static size_t value_constructions() { return get_metric<Op::ValueConstruction>(); }

    template <typename TargetT>
    inline static constexpr bool w_has_policy_v = detail::has_policy_v<TargetT, Policies...>;

    using value_t = detail::get_value_type_t<int, Policies...>;
    value_t value;

    static_assert((detail::is_policy_v<Policies> && ...), "all types in Policies must inherit from policy_base");
    static_assert(!detail::contains_multiple_value_policies_v<Policies...>, "Widget must contain at most 1 Value policy");

    /**
     * @brief Helper member to indicate if this widget can be move constructed
     */
    inline static constexpr bool can_move_construct =
      !w_has_policy_v<policy::NoMove> &&
      !w_has_policy_v<policy::Disable<Op::MoveConstruction>> &&
      std::is_move_constructible_v<value_t>;

    /**
     * @brief Helper member to indicate if this widget can be move assigned
     */
    inline static constexpr bool can_move_assign =
      !w_has_policy_v<policy::NoMove> &&
      !w_has_policy_v<policy::Disable<Op::MoveAssignment>> &&
      std::is_move_assignable_v<value_t>;

    /**
     * @brief Helper member to indicate if this widget can be nothrow move constructed
     */
    inline static constexpr bool can_nothrow_move_construct =
      !w_has_policy_v<policy::NoMove> &&
      !w_has_policy_v<policy::ThrowOn<Op::MoveConstruction>> &&
      !w_has_policy_v<policy::Disable<Op::MoveConstruction>> &&
      detail::get_throwat_n_v<Op::MoveConstruction, Policies...> == 0 &&
      std::is_nothrow_move_constructible_v<value_t>;

    /**
     * @brief Helper member to indicate if this widget can be nothrow move assigned
     */
    inline static constexpr bool can_nothrow_move_assign =
      !w_has_policy_v<policy::NoMove> &&
      !w_has_policy_v<policy::ThrowOn<Op::MoveAssignment>> &&
      !w_has_policy_v<policy::Disable<Op::MoveAssignment>> &&
      detail::get_throwat_n_v<Op::MoveAssignment, Policies...> == 0 &&
      std::is_nothrow_move_constructible_v<value_t>;

    /**
     * @brief Helper member to indicate if this widget can be copy constructed
     */
    inline static constexpr bool can_copy_construct =
      !w_has_policy_v<policy::NoCopy> &&
      !w_has_policy_v<policy::Disable<Op::CopyConstruction>> &&
      std::is_copy_constructible_v<value_t>;

    /**
     * @brief Helper member to indicate if this widget can be copy assigned
     */
    inline static constexpr bool can_copy_assign =
      !w_has_policy_v<policy::NoCopy> &&
      !w_has_policy_v<policy::Disable<Op::CopyAssignment>> &&
      std::is_copy_assignable_v<value_t>;

    /**
     * @brief Helper member to indicate if this widget can be default constructed
     */
    inline static constexpr bool can_default_construct =
      !w_has_policy_v<policy::Disable<Op::DefaultConstruction>> &&
      std::is_default_constructible_v<value_t>;

    /**
     * @brief Reset all tracked metric counters to 0 for this Widget type.
     */
    static void reset_metrics() {
      metrics_.fill(0);
    }

    /**
     * @brief Default constructor. Default constructs value and records this in the session.
     * ThrowOnDefaultConstruction widgets will throw without session tracking.
     */
    Widget() requires(can_default_construct) {
      throw_if_needed<Op::DefaultConstruction>();
      get_metric<Op::DefaultConstruction>()++;
      get_metric<Op::ActiveInstance>()++;
    }

    /**
     * @brief lvalue Value construction. Copy constructs value and records this in the session.
     */
    explicit Widget(const value_t& x) : value{x} {
      get_metric<Op::ValueConstruction>()++;
      get_metric<Op::ActiveInstance>()++;
    }

    /**
     * @brief rvalue Value construction. Move constructs value and records this in the session.
     */
    explicit Widget(value_t&& x) : value{std::move(x)} {
      get_metric<Op::ValueConstruction>()++;
      get_metric<Op::ActiveInstance>()++;
    }

    /**
     * @brief Destructor. Records this in the session.
     */
    ~Widget() {
      get_metric<Op::Destruction>()++;
      get_metric<Op::ActiveInstance>()--;
    }

    /**
     * @brief Move constructor. Move constructs value and records this in the session.
     * ThrowOnMove widgets will throw before moving from other's value, without session tracking.
     * Move construction is noexcept if the widget is not ThrowOnMove and value_t is nothrow move constructible.
     */
    Widget(Widget&& other)
      noexcept(can_nothrow_move_construct)
      requires(can_move_construct) :
      value{(throw_if_needed<Op::MoveConstruction>(), std::move(other.value))}
    {
      get_metric<Op::MoveConstruction>()++;
      get_metric<Op::ActiveInstance>()++;
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
      noexcept(can_nothrow_move_assign)
      requires(can_move_assign) {
      throw_if_needed<Op::MoveAssignment>();
      get_metric<Op::MoveAssignment>()++;
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
    Widget(const Widget& other) requires(can_copy_construct) :
      value{(throw_if_needed<Op::CopyConstruction>(), other.value)}
    {
      get_metric<Op::CopyConstruction>()++;
      get_metric<Op::ActiveInstance>()++;
    }

    /**
     * @brief Copy assignment operator. Copy assigns value and records this in the session.
     * ThrowOnCopy widgets will throw before value is assigned, without session tracking.
     */
    Widget& operator=(const Widget& other) requires(can_copy_assign) {
      throw_if_needed<Op::CopyAssignment>();
      get_metric<Op::CopyAssignment>()++;
      value = other.value;
      return *this;
    }

  private:
    inline static thread_local std::array<size_t, static_cast<uint8_t>(Op::_COUNT)> metrics_{};
    
    template <Op op>
    inline static void throw_if_needed() {
      using namespace policy;
      if constexpr (w_has_policy_v<policy::ThrowOn<op>>) {
        throw WidgetException(std::format("uwidget: ThrowOn {}", op_message_v<op>).c_str());
      }
      else {
        constexpr size_t limit = detail::get_throwat_n_v<op, Policies...>;
        if constexpr (limit > 0) {
          if (get_metric<op>() == limit - 1) throw WidgetException(std::format("uwidget: {} {}s reached", limit, op_message_v<op>).c_str());
        }
      }
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
