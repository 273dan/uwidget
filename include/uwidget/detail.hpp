#pragma once

#include <type_traits>
#include "policy.hpp"

namespace uwidget::detail {
  template<typename TargetT, typename FirstT, typename ...Policies>
  inline constexpr bool has_policy_v =
  std::is_same_v<TargetT, FirstT> ||
  (std::is_same_v<TargetT, Policies> || ...);

  using namespace policy;
  template<typename Target>
  inline constexpr bool is_policy_v =
    std::is_base_of_v<policy::policy_base, Target>;
}
