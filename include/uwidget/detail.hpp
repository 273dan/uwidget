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
