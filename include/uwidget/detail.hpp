#pragma once

#include <type_traits>
#include "policy.hpp"

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
  



}
