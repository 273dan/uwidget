#include "uwidget/uwidget.hpp"
#include "doctest/doctest.h"
#include <type_traits>

using namespace uwidget;
using namespace uwidget::policy;

TEST_CASE("assignability and constructibility") {


  SUBCASE("no copy") {
    using W = Widget<NoCopy>;
    CHECK( ! std::is_copy_assignable_v<W>);
    CHECK( ! std::is_copy_constructible_v<W>);
    CHECK( ! std::is_nothrow_copy_assignable_v<W>);
    CHECK( ! std::is_nothrow_copy_constructible_v<W>);
  }

  SUBCASE("no move") {
    using W = Widget<NoMove>;
    CHECK( ! std::is_move_assignable_v<W>);
    CHECK( ! std::is_move_constructible_v<W>);
    CHECK( ! std::is_nothrow_move_assignable_v<W>);
    CHECK( ! std::is_nothrow_move_constructible_v<W>);
  }

  SUBCASE("ThrowOn copy construct") {
    using W = Widget<ThrowOnNth<Op::CopyConstruction, 1>>;
    CHECK(   std::is_copy_assignable_v<W>);
    CHECK(   std::is_copy_constructible_v<W>);
    CHECK(   std::is_nothrow_copy_assignable_v<W>);
    CHECK( ! std::is_nothrow_copy_constructible_v<W>);
  }

  SUBCASE("ThrowOn copy assign") {
    using W = Widget<ThrowOnNth<Op::CopyAssignment, 1>>;
    CHECK(   std::is_copy_assignable_v<W>);
    CHECK(   std::is_copy_constructible_v<W>);
    CHECK( ! std::is_nothrow_copy_assignable_v<W>);
    CHECK(   std::is_nothrow_copy_constructible_v<W>);
  }

  SUBCASE("ThrowOn move construct") {
  using W = Widget<ThrowOnNth<Op::MoveConstruction, 1>>;
    CHECK(   std::is_move_assignable_v<W>);
    CHECK(   std::is_move_constructible_v<W>);
    CHECK(   std::is_nothrow_move_assignable_v<W>);
    CHECK( ! std::is_nothrow_move_constructible_v<W>);
  }

  SUBCASE("ThrowOn move assign") {
    using W = Widget<ThrowOnNth<Op::MoveAssignment, 1>>;
    CHECK(   std::is_move_assignable_v<W>);
    CHECK(   std::is_move_constructible_v<W>);
    CHECK( ! std::is_nothrow_move_assignable_v<W>);
    CHECK(   std::is_nothrow_move_constructible_v<W>);
  }

  SUBCASE("ThrowOn default construct") {
    using W = Widget<ThrowOnNth<Op::DefaultConstruction, 1>>;
    CHECK(   std::is_default_constructible_v<W>);
    CHECK( ! std::is_nothrow_default_constructible_v<W>);
  }

  SUBCASE("ThrowOn value construct") {
    using W = Widget<ThrowOnNth<Op::ValueConstruction, 1>, Value<int>>;
    CHECK(   std::is_constructible_v<W, int>);
    CHECK( ! std::is_nothrow_constructible_v<W, int>);
  }

  SUBCASE("no constructibliity or assignability constraints") {
    using W = Widget<>;
    CHECK(   std::is_move_assignable_v<W>);
    CHECK(   std::is_move_constructible_v<W>);
    CHECK(   std::is_nothrow_move_assignable_v<W>);
    CHECK(   std::is_nothrow_move_constructible_v<W>);
    CHECK(   std::is_copy_assignable_v<W>);
    CHECK(   std::is_copy_constructible_v<W>);
    CHECK(   std::is_nothrow_copy_assignable_v<W>);
    CHECK(   std::is_nothrow_copy_constructible_v<W>);
  }

  SUBCASE("where value type is non moveable") {

    struct NonMovingValue {
      NonMovingValue(NonMovingValue&&) = delete;
      NonMovingValue& operator=(NonMovingValue&&) = delete;
    };

    using W = Widget<Value<NonMovingValue>>;
    CHECK( ! std::is_move_assignable_v<W>);
    CHECK( ! std::is_move_constructible_v<W>);
    CHECK( ! std::is_nothrow_move_assignable_v<W>);
    CHECK( ! std::is_nothrow_move_constructible_v<W>);

  }

  SUBCASE("where value type is non nothrow moveable") {

    struct NonMovingValue {
      NonMovingValue(NonMovingValue&&) noexcept(false) = default;
      NonMovingValue& operator=(NonMovingValue&&) noexcept(false) = default;
    };

    using W = Widget<Value<NonMovingValue>>;
    CHECK(   std::is_move_assignable_v<W>);
    CHECK(   std::is_move_constructible_v<W>);
    CHECK( ! std::is_nothrow_move_assignable_v<W>);
    CHECK( ! std::is_nothrow_move_constructible_v<W>);

  }

  SUBCASE("where value type is non default constructible") {
    struct NonDefaultConstructibleValue {
      NonDefaultConstructibleValue() = delete;
    };

    using W = Widget<Value<NonDefaultConstructibleValue>>;

    CHECK( ! std::is_default_constructible_v<W>);
    CHECK( ! std::is_nothrow_default_constructible_v<W>);

  }

  SUBCASE("force non-noexcept") {
    SUBCASE("default construction") {
      using W = Widget<ForceNonNoexcept<Op::DefaultConstruction>>;
      CHECK( ! std::is_nothrow_default_constructible_v<W>);
    }
    SUBCASE("value construction") {
      using W = Widget<ForceNonNoexcept<Op::ValueConstruction>>;
      CHECK( ! std::is_nothrow_constructible_v<W, int>);
    }
    SUBCASE("copy construction") {
      using W = Widget<ForceNonNoexcept<Op::CopyConstruction>>;
      CHECK( ! std::is_nothrow_copy_constructible_v<W>);
    }
    SUBCASE("move construction") {
      using W = Widget<ForceNonNoexcept<Op::MoveConstruction>>;
      CHECK( ! std::is_nothrow_move_constructible_v<W>);
    }
    SUBCASE("copy assignment") {
      using W = Widget<ForceNonNoexcept<Op::CopyAssignment>>;
      CHECK( ! std::is_nothrow_copy_assignable_v<W>);
    }
    SUBCASE("move assignment") {
      using W = Widget<ForceNonNoexcept<Op::MoveAssignment>>;
      CHECK( ! std::is_nothrow_move_assignable_v<W>);
    }

  }

}

TEST_CASE("policy validation") {
  REQUIRE_MESSAGE(
    Widget<Value<float>>::w_has_policy_v<Value<float>>,
    "has_policy_v must work for the following tests"
  );
  SUBCASE("illegal policy packs") {
    SUBCASE("non policy template detection") {
      CHECK( ! detail::is_policy_v<int>);
      CHECK( detail::is_policy_v<Value<float>>);
    }
    SUBCASE("multiple value policy detection") {
      CHECK( ! detail::contains_multiple_value_policies_v<NoMove, ForceNonNoexcept<Op::DefaultConstruction>>);
      CHECK( ! detail::contains_multiple_value_policies_v<Value<float>, NoCopy>);
      CHECK(   detail::contains_multiple_value_policies_v<Value<float>, Value<int>>);

    }

  }


  SUBCASE("polciy expansion") {
    SUBCASE("NoMove expansion") {
      CHECK(Widget<NoMove>::w_has_policy_v<Disable<Op::MoveConstruction>>);
      CHECK(Widget<NoMove>::w_has_policy_v<Disable<Op::MoveAssignment>>);
    }
    SUBCASE("NoCopy expansion") {
      CHECK(Widget<NoCopy>::w_has_policy_v<Disable<Op::CopyConstruction>>);
      CHECK(Widget<NoCopy>::w_has_policy_v<Disable<Op::CopyAssignment>>);
    }
  }
}
