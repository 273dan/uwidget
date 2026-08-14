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


    auto& get_session() {
      return Session<Widget>::instance();
    }

    

    // default construction
    Widget() requires(can_default_construct) {
      get_session().template reg<RegisteredData::DefaultConstructions>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }
    Widget() requires(!can_default_construct) = delete;

    // destruction
    ~Widget() {
      get_session().template reg<RegisteredData::Destructions>();
      get_session().template dereg<RegisteredData::ActiveInstances>();
    }

    // move construction
    Widget(Widget&&) requires(can_move) {
      get_session().template reg<RegisteredData::MoveConstructions>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }
    Widget(Widget&&) requires(!can_move) = delete;

    // move assignment
    Widget& operator=(Widget&&) requires(can_move) {
      get_session().template reg<RegisteredData::MoveAssignments>();
      return *this;
    }
    Widget& operator=(Widget&&) requires(!can_move) = delete;

    // copy construction
    Widget(const Widget&) requires(can_copy) {
      get_session().template reg<RegisteredData::CopyConstructions>();
      get_session().template reg<RegisteredData::ActiveInstances>();
    }
    Widget(const Widget&) requires(!can_copy) = delete;

    // copy assignment
    Widget& operator=(const Widget&) requires(can_copy) {
      get_session().template reg<RegisteredData::CopyAssignments>();
      return *this;
    }
    Widget& operator=(const Widget&) requires(!can_copy) = delete;

  };

}
