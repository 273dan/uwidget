#pragma once

#include "session.hpp"
#include <type_traits>

namespace uwidget {
  namespace policy {
    struct NoMove{};
    struct NoCopy{};
    struct NoDefault{};
  }
  namespace detail {
    template<typename Target, typename ...Policies>
    inline constexpr bool has_policy_v = (std::is_same_v<Target, Policies> || ...);

  }
  template <typename ...Policies>
  class Widget {
    Session* session_;
    Widget() requires(!detail::has_policy_v<policy::NoDefault>){}

    Widget(Widget&&) requires(!detail::has_policy_v<policy::NoMove>) {}
    Widget& operator=(Widget&&) requires(!detail::has_policy_v<policy::NoMove, Policies...>) {}

    Widget(const Widget&) requires(!detail::has_policy_v<policy::NoCopy, Policies...>) {}
    Widget& operator=(const Widget&) requires(!detail::has_policy_v<policy::NoCopy, Policies...>) {}


  };

}
