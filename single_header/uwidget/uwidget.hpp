// uwidget.hpp -- generated 08/16/26 17:11:38
#pragma once

#include <array>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
// begin operation.hpp -----------------


namespace uwidget {

enum class Op: uint8_t {
  ActiveInstance = 0,
  Destruction,
  DefaultConstruction,
  CopyConstruction,
  CopyAssignment,
  MoveConstruction,
  MoveAssignment,
  ValueConstruction,
  _COUNT
};
}
// end operation.hpp -----------------
// begin policy.hpp -----------------

namespace uwidget::policy {

  /**
   * @brief Base class determining if a struct is a policy
   */
  struct policy_base {};
  

  /**
   * @brief Determines the type of the Widget's member. Defaults to int
   */
  template <typename T>
  struct Value : policy_base{};

  /**
   * @brief Deletes move both operations
   */
  struct NoMove : policy_base{};

  /**
   * @brief Deletes copy both operations
   */
  struct NoCopy : policy_base{};

  /**
   * @brief Deletes default constructor
   */
  struct NoDefaultConstruct : policy_base{};

  /**
   * @brief Causes move both operations to throw
   */
  struct ThrowOnMove : policy_base{};

  /**
   * @brief Causes copy both operations to throw
   */
  struct ThrowOnCopy : policy_base{};

  /**
   * @brief Causes default construction to throw
   */
  struct ThrowOnDefaultConstruction : policy_base{};

  /**
   * @brief Causes the Nth instantiation of the specified operation to throw
   */
  template <Op op, size_t N>
  struct ThrowOnNthOperation : policy_base{};

}
// end policy.hpp -----------------
// begin widget_exception.hpp -----------------


namespace uwidget {
class WidgetException : public std::runtime_error {
  public:
    explicit WidgetException(const char* msg) : runtime_error{msg} {};
  };

}
// end widget_exception.hpp -----------------
// begin detail.hpp -----------------


namespace uwidget::detail {

  template<typename Target, typename ...Policies>
  inline constexpr bool has_policy_v =
    (std::is_same_v<Target, Policies> || ...);

  template<typename Target>
  inline constexpr bool is_policy_v =
    std::is_base_of_v<policy::policy_base, Target>;


  template <typename Default, typename ...Policies>
  struct get_value_type {
    using type = Default;
  };

  template <typename Default, typename T, typename ...Other>
  struct get_value_type<Default, policy::Value<T>, Other...> {
    using type = T;
  };

  template <typename Default, typename ...Other>
  using get_value_type_t = typename get_value_type<Default, Other...>::type;

  template<typename Target>
  struct is_value_policy : std::false_type {};

  template<typename T>
  struct is_value_policy<policy::Value<T>> : std::true_type {};

  template <typename Target>
  inline constexpr bool is_value_policy_v = is_value_policy<Target>::value;

  template <typename ...Policies>
  inline constexpr bool contains_multiple_value_policies_v =
    ((is_value_policy_v<Policies> ? 1 : 0) + ... + 0) >= 2;

  template <Op OpV, typename ...Policies>
  struct get_throwat_n {
    static constexpr size_t n = 0;
  };

  template <Op OpV, size_t N, typename ...Other>
  struct get_throwat_n<OpV, policy::ThrowOnNthOperation<OpV, N>, Other...> {
    static constexpr size_t n = N;
  };

  template <Op OpV, typename First, typename ...Other>
  struct get_throwat_n<OpV, First, Other...> {
    static constexpr size_t n = get_throwat_n<OpV, Other...>::n;
  };

  template <Op OpV, typename ...Policies>
  inline constexpr size_t get_throwat_n_v = get_throwat_n<OpV, Policies...>::n;
  



}
// end detail.hpp -----------------
// begin widget.hpp -----------------


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
     * @brief Helper member to indicate if this widget can be moved
     */
    inline static constexpr bool can_move =
      !w_has_policy_v<policy::NoMove> &&
      std::is_move_constructible_v<value_t> &&
      std::is_move_assignable_v<value_t>;

    /**
     * @brief Helper member to indicate if this widget can be moved
     */
    inline static constexpr bool can_nothrow_move =
      !w_has_policy_v<policy::NoMove> &&
      !w_has_policy_v<policy::ThrowOnMove> &&
      detail::get_throwat_n_v<Op::MoveAssignment, Policies...> == 0 &&
      detail::get_throwat_n_v<Op::MoveConstruction, Policies...> == 0 &&
      std::is_nothrow_move_constructible_v<value_t> &&
      std::is_nothrow_move_assignable_v<value_t>;

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
      metrics_.fill(0);
    }

    /**
     * @brief Default constructor. Default constructs value and records this in the session.
     * ThrowOnDefaultConstruction widgets will throw without session tracking.
     */
    Widget() requires(can_default_construct) {
      if (should_throw<Op::DefaultConstruction>()) {
        throw WidgetException("uwidget: ThrowOnDefaultconstruction");
      }
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
      noexcept(can_nothrow_move)
      requires(can_move) :
      value{should_throw<Op::MoveConstruction>() ? throw WidgetException("uwidget: ThrowOnMove")
                                                 : std::move(other.value)}
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
      noexcept(can_nothrow_move)
      requires(can_move) {
      if (should_throw<Op::MoveAssignment>()) {
        throw WidgetException("uwidget: ThrowOnMove");
      }
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
    Widget(const Widget& other) requires(can_copy) :
      value{should_throw<Op::CopyConstruction>() ? throw WidgetException("uwidget: ThrowOnCopy")
                                                 : other.value}
    {
      get_metric<Op::CopyConstruction>()++;
      get_metric<Op::ActiveInstance>()++;
    }

    /**
     * @brief Copy assignment operator. Copy assigns value and records this in the session.
     * ThrowOnCopy widgets will throw before value is assigned, without session tracking.
     */
    Widget& operator=(const Widget& other) requires(can_copy) {
      if (should_throw<Op::CopyAssignment>()) {
        throw WidgetException("uwidget: ThrowOnCopy");
      }
      get_metric<Op::CopyAssignment>()++;
      value = other.value;
      return *this;
    }

  private:
    inline static thread_local std::array<size_t, static_cast<uint8_t>(Op::_COUNT)> metrics_{};
    
    template <Op op>
    inline static bool should_throw() {
      using namespace policy;
      if constexpr (w_has_policy_v<ThrowOnCopy> && (op == Op::CopyAssignment || op == Op::CopyConstruction)) {
        return true;
      }
      else if constexpr (w_has_policy_v<ThrowOnMove> && (op == Op::MoveAssignment || op == Op::MoveConstruction)) {
        return true;
      }
      else if constexpr (w_has_policy_v<ThrowOnDefaultConstruction> && op == Op::DefaultConstruction) {
        return true;
      }
      else {
        constexpr size_t limit = detail::get_throwat_n_v<op, Policies...>;
        if constexpr (limit > 0) {
          if (get_metric<op>() == limit - 1) return true;
        }
      }
      return false;
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
// end widget.hpp -----------------
