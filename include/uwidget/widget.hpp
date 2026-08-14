#pragma once

#include "session.hpp"
#include <type_traits>

namespace uwidget {
  namespace policy {
    struct NoMove{};
    struct NoCopy{};
    struct NoDefaultConstruct{};
  }
  namespace detail {
    template<typename Target, typename ...Policies>
    inline constexpr bool has_policy_v = (std::is_same_v<Target, Policies> || ...);

  }
  template <typename ...Policies>
  class Widget {
    public:

    inline static constexpr bool can_move = !detail::has_policy_v<policy::NoMove, Policies...>;
    inline static constexpr bool can_copy = !detail::has_policy_v<policy::NoCopy, Policies...>;
    inline static constexpr bool can_default_construct = !detail::has_policy_v<policy::NoDefaultConstruct, Policies...>;



    

    // default construction
    Widget() requires(can_default_construct) = default;
    Widget() requires(!can_default_construct) = delete;

    // move construction
    Widget(Widget&&) requires(can_move) = default;
    Widget(Widget&&) requires(!can_move) = delete;

    // move assignment
    Widget& operator=(Widget&&) requires(can_move) = default;
    Widget& operator=(Widget&&) requires(!can_move) = delete;

    // copy construction
    Widget(const Widget&) requires(can_copy) = default;
    Widget(const Widget&) requires(!can_copy) = delete;

    // copy assignment
    Widget& operator=(const Widget&) requires(can_copy) = default;
    Widget& operator=(const Widget&) requires(!can_copy) = delete;


    private:
    Session* session_;
  };

}
