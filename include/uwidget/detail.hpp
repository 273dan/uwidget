#pragma once

#include <tuple>
#include <type_traits>
#include "policy.hpp"
#include "operation.hpp"

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

// policy filtering and matching -----
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
// -----

// widget traits -----
  template <typename Tuple>
  struct widget_traits_impl;

  template <typename ...ExpandedPolicies>
  struct widget_traits_impl<std::tuple<ExpandedPolicies...>> {
    using value_t = get_value_type_t<int, ExpandedPolicies...>;

    /**
     * @brief Indicates if this widget can be move constructed
     */
    inline static constexpr bool can_move_construct =
      !has_policy_v<policy::Disable<Op::MoveConstruction>, ExpandedPolicies...> &&
      std::is_move_constructible_v<value_t>;

    /**
     * @brief Indicates if this widget can be move assigned
     */
    inline static constexpr bool can_move_assign =
      !has_policy_v<policy::Disable<Op::MoveAssignment>, ExpandedPolicies...> &&
      std::is_move_assignable_v<value_t>;

    /**
     * @brief Indicates if this widget can be nothrow move constructed
     */
    inline static constexpr bool can_nothrow_move_construct =
      can_move_construct &&
      !has_policy_v<policy::ThrowOn<Op::MoveConstruction>, ExpandedPolicies...> &&
      get_throwat_n_v<Op::MoveConstruction, ExpandedPolicies...> == 0 &&
      std::is_nothrow_move_constructible_v<value_t>;

    /**
     * @brief Indicates if this widget can be nothrow move assigned
     */
    inline static constexpr bool can_nothrow_move_assign =
      can_move_assign &&
      !has_policy_v<policy::ThrowOn<Op::MoveAssignment>, ExpandedPolicies...> &&
      get_throwat_n_v<Op::MoveAssignment, ExpandedPolicies...> == 0 &&
      std::is_nothrow_move_assignable_v<value_t>;

    /**
     * @brief Indicates if this widget can be copy constructed
     */
    inline static constexpr bool can_copy_construct =
      !has_policy_v<policy::Disable<Op::CopyConstruction>, ExpandedPolicies...> &&
      std::is_copy_constructible_v<value_t>;

    /**
     * @brief Indicates if this widget can be copy assigned
     */
    inline static constexpr bool can_copy_assign =
      !has_policy_v<policy::Disable<Op::CopyAssignment>, ExpandedPolicies...> &&
      std::is_copy_assignable_v<value_t>;

    /**
     * @brief Indicates if this widget can be default constructed
     */
    inline static constexpr bool can_default_construct =
      !has_policy_v<policy::Disable<Op::DefaultConstruction>, ExpandedPolicies...> &&
      std::is_default_constructible_v<value_t>;
  };

  template <typename ...Policies>
  using widget_traits = widget_traits_impl<expanded_policy_tuple_t<Policies...>>;
// -----



}
