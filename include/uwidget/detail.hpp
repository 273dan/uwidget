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
    std::is_same_v<Target, NoDefaultConstruct> ||
    std::is_same_v<Target, NoCopy> ||
    std::is_same_v<Target, NoMove> ||
    std::is_same_v<Target, ThrowOnDefaultConstruction> ||
    std::is_same_v<Target, ThrowOnCopy> ||
    std::is_same_v<Target, ThrowOnMove>;
}
