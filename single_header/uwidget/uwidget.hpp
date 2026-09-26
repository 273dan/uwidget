// uwidget.hpp -- generated 09/26/26 20:23:21
#pragma once

#include <array>
#include <atomic>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <stdexcept>
#include <string_view>
#include <tuple>
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

template <Op OpV> 
inline constexpr std::string_view op_message_v =
  "Unknown Operation";

template <> 
inline constexpr std::string_view op_message_v<Op::ActiveInstance> =
  "Active Instance";

template <> 
inline constexpr std::string_view op_message_v<Op::Destruction> =
  "Destruction";

template <> 
inline constexpr std::string_view op_message_v<Op::DefaultConstruction> =
  "Default Construction";

template <> 
inline constexpr std::string_view op_message_v<Op::CopyConstruction> =
  "Copy Construction";

template <> 
inline constexpr std::string_view op_message_v<Op::CopyAssignment> =
  "Copy Assignment";

template <> 
inline constexpr std::string_view op_message_v<Op::MoveConstruction> =
  "Move Construction";

template <> 
inline constexpr std::string_view op_message_v<Op::MoveAssignment> =
  "Move Assignment";

template <> 
inline constexpr std::string_view op_message_v<Op::ValueConstruction> =
  "Value Construction";

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
   * @brief Deletes the operation specified by OpV
   */
  template <Op OpV>
  struct Disable : policy_base{};

  /**
   * @brief Disables both move operations
   * This is a shorthand for Disable<Op::MoveConstruction>, Disable<Op::MoveAssignment>
   */
  struct NoMove : policy_base{};

  /**
   * @brief Deletes copy both operations
   * This is a shorthand for Disable<Op::CopyConstruction>, Disable<Op::CopyAssignment>
   */
  struct NoCopy : policy_base{};

  /**
   * @brief Causes the operation specified by OpV to throw
   */
  template <Op OpV>
  struct ThrowOn : policy_base{};

  /**
   * @brief Causes the Nth instantiation of the specified operation to throw
   */
  template <Op op, size_t N>
  struct ThrowOnNthOperation : policy_base{};


  /**
   * @brief Forces the specified operation to be non-noexcept
   */
  template <Op op>
  struct ForceNonNoexcept : policy_base{};

  /**
   * @brief Dummy policy only used to differentiate otherwise identical Widgets in the same scope
   */
  template <int n = 0>
  struct Id : policy_base{};

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


// policy expansion -----


  /**
   * @brief Default expanded policy wraps single policy in tuple
   */
  template <typename Policy>
  struct expand_policy {
    using type = std::tuple<Policy>;
  };

  /**
   * @brief Type template for expand_policy
   */
  template <typename Policy>
  using expand_policy_t = typename expand_policy<Policy>::type;

  /**
   * @brief Defines the type of the collection of policies after expansion
   */
  template <typename ...Policies>
    using expanded_policy_tuple_t = decltype(std::tuple_cat(expand_policy_t<Policies>{}...));

  /**
   * @brief NoCopy expands into Disable copy construction and assignment
   */
  template <>
  struct expand_policy<policy::NoCopy> {
    using type = std::tuple<
      policy::Disable<Op::CopyConstruction>,
      policy::Disable<Op::CopyAssignment>
    >;
  };

  /**
   * @brief NoMove expands into Disable move construction and assignment
   */
  template <>
  struct expand_policy<policy::NoMove> {
    using type = std::tuple<
      policy::Disable<Op::MoveConstruction>,
      policy::Disable<Op::MoveAssignment>
    >;
  };

// -----
  
  
  
  

// type traits helpers -----

  template <Op op, typename T>
  struct get_type_trait_t {};

  template <typename T>
  struct get_type_trait_t<Op::CopyConstruction, T> {
    constexpr static bool has_ability = std::is_copy_constructible_v<T>;
    constexpr static bool has_nothrow_ability = std::is_nothrow_copy_constructible_v<T>;
  };

  template <typename T>
  struct get_type_trait_t<Op::CopyAssignment, T> {
    constexpr static bool has_ability = std::is_copy_assignable_v<T>;
    constexpr static bool has_nothrow_ability = std::is_nothrow_copy_assignable_v<T>;
  };

  template <typename T>
  struct get_type_trait_t<Op::MoveConstruction, T> {
    constexpr static bool has_ability = std::is_move_constructible_v<T>;
    constexpr static bool has_nothrow_ability = std::is_nothrow_move_constructible_v<T>;
  };

  template <typename T>
  struct get_type_trait_t<Op::MoveAssignment, T> {
    constexpr static bool has_ability = std::is_move_assignable_v<T>;
    constexpr static bool has_nothrow_ability = std::is_nothrow_move_assignable_v<T>;
  };

  template <typename T>
  struct get_type_trait_t<Op::DefaultConstruction, T> {
    constexpr static bool has_ability = std::is_default_constructible_v<T>;
    constexpr static bool has_nothrow_ability = std::is_nothrow_default_constructible_v<T>;
  };

  template <Op op, typename T>
  constexpr static bool has_ability_v = get_type_trait_t<op, T>::has_ability;

  template <Op op, typename T>
  constexpr static bool has_nothrow_ability_v = get_type_trait_t<op, T>::has_nothrow_ability;

  



// -----

// policy filtering and matching -----

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

  template <typename Default, typename First, typename ...Other>
  struct get_value_type<Default, First, Other...> {
    using type = typename get_value_type<Default, Other...>::type;
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
// -----

// widget traits -----

  template <typename Tuple>
  struct widget_traits_impl;

  template <typename ...ExpandedPolicies>
  struct widget_traits_impl<std::tuple<ExpandedPolicies...>> {

    using value_t = get_value_type_t<int, ExpandedPolicies...>;

    template <typename Target>
    inline static constexpr bool has_policy_v = (std::is_same_v<Target, ExpandedPolicies> || ...);

    template <Op op>
    inline static constexpr bool can_op_v =
      !has_policy_v<policy::Disable<op>> &&
      has_ability_v<op, value_t>;

    template <Op op>
    inline static constexpr bool can_nothrow_op_v =
         !has_policy_v<policy::ForceNonNoexcept<op>>
      && !has_policy_v<policy::Disable<op>>
      && !has_policy_v<policy::ThrowOn<op>>
      && has_ability_v<op, value_t> 
      && has_nothrow_ability_v<op, value_t>
      && get_throwat_n_v<op, ExpandedPolicies...> == 0;


  };

  template <typename ...Policies>
  using widget_traits = widget_traits_impl<expanded_policy_tuple_t<Policies...>>;
// -----



}
// end detail.hpp -----------------
// begin widget.hpp -----------------


namespace uwidget {
  
  template <typename ...Policies>
  class Widget {
  public:

    template <Op op>
    static size_t get_metric() {
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


    using value_t = detail::get_value_type_t<int, Policies...>;
    value_t value;

    static_assert((detail::is_policy_v<Policies> && ...), "all types in Policies must inherit from policy_base");
    static_assert(!detail::contains_multiple_value_policies_v<Policies...>, "Widget must contain at most 1 Value policy");

    using traits = detail::widget_traits<Policies...>;

    template <typename TargetT>
    inline static constexpr bool w_has_policy_v = traits::template has_policy_v<TargetT>;



    /**
     * @brief Reset all tracked metric counters to 0 for this Widget type.
     */
    static void reset_metrics() {
      for(auto& i : metrics_) {
        i.store(0);

      }
    }

    /**
     * @brief Default constructor. Default constructs value and records this in the session.
     * ThrowOnDefaultConstruction widgets will throw without session tracking.
     */
    Widget() requires(traits::template can_op_v<Op::DefaultConstruction>) {
      throw_if_needed<Op::DefaultConstruction>();
      get_metric_mut<Op::DefaultConstruction>()++;
      get_metric_mut<Op::ActiveInstance>()++;
    }

    /**
     * @brief lvalue Value construction. Copy constructs value and records this in the session.
     */
    explicit Widget(const value_t& x) : value{x} {
      get_metric_mut<Op::ValueConstruction>()++;
      get_metric_mut<Op::ActiveInstance>()++;
    }

    /**
     * @brief rvalue Value construction. Move constructs value and records this in the session.
     */
    explicit Widget(value_t&& x) : value{std::move(x)} {
      get_metric_mut<Op::ValueConstruction>()++;
      get_metric_mut<Op::ActiveInstance>()++;
    }

    /**
     * @brief Destructor. Records this in the session.
     */
    ~Widget() {
      get_metric_mut<Op::Destruction>()++;
      get_metric_mut<Op::ActiveInstance>()--;
    }

    /**
     * @brief Move constructor. Move constructs value and records this in the session.
     * ThrowOnMove widgets will throw before moving from other's value, without session tracking.
     * Move construction is noexcept if the widget is not ThrowOnMove and value_t is nothrow move constructible.
     */
    Widget(Widget&& other)
      noexcept(traits::template can_nothrow_op_v<Op::MoveConstruction>) 
      requires(traits::template can_op_v<Op::MoveConstruction>) :
      value{(throw_if_needed<Op::MoveConstruction>(), std::move(other.value))}
    {
      get_metric_mut<Op::MoveConstruction>()++;
      get_metric_mut<Op::ActiveInstance>()++;
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
      noexcept(traits::template can_nothrow_op_v<Op::MoveAssignment>)
      requires(traits::template can_op_v<Op::MoveConstruction>) {
      throw_if_needed<Op::MoveAssignment>();
      get_metric_mut<Op::MoveAssignment>()++;
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
    Widget(const Widget& other) requires(traits::template can_op_v<Op::CopyConstruction>) :
      value{(throw_if_needed<Op::CopyConstruction>(), other.value)}
    {
      get_metric_mut<Op::CopyConstruction>()++;
      get_metric_mut<Op::ActiveInstance>()++;
    }

    /**
     * @brief Copy assignment operator. Copy assigns value and records this in the session.
     * ThrowOnCopy widgets will throw before value is assigned, without session tracking.
     */
    Widget& operator=(const Widget& other) requires(traits::template can_op_v<Op::CopyAssignment>) {
      throw_if_needed<Op::CopyAssignment>();
      get_metric_mut<Op::CopyAssignment>()++;
      value = other.value;
      return *this;
    }

  private:
    inline static std::array<std::atomic<size_t>, static_cast<uint8_t>(Op::_COUNT)> metrics_{};
    
    template <Op op>
    static std::atomic<size_t>& get_metric_mut() {
      return metrics_[static_cast<uint8_t>(op)];
    }

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
// end widget.hpp -----------------
