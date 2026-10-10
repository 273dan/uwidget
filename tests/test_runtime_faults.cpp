#include "doctest/doctest.h"
#include "uwidget/uwidget.hpp"

using namespace uwidget;
using namespace uwidget::policy;

TEST_CASE("throw on nth operation") {

  SUBCASE("default construction") {
    SUBCASE("n = 3") {
      using W = Widget<Unique<>, ThrowOnNth<Op::DefaultConstruction, 3>, Value<int>>;
      CHECK_NOTHROW(W{});
      CHECK_NOTHROW(W{});
      CHECK_THROWS(W{});
    }
    SUBCASE("n = 1") {
      using W = Widget<Unique<>, ThrowOnNth<Op::DefaultConstruction, 1>, Value<int>>;
      CHECK_THROWS(W{});
    }
  }

  SUBCASE("value construction") {
    using W = Widget<Unique<>, ThrowOnNth<Op::ValueConstruction, 3>, Value<int>>;
    CHECK_NOTHROW(W{10});
    CHECK_NOTHROW(W{10});
    CHECK_THROWS(W{10});
  }

  SUBCASE("copy construction") {
    using W = Widget<Unique<>, ThrowOnNth<Op::CopyConstruction, 3>, Value<int>>;
    W w1{};
    CHECK_NOTHROW(W{w1});
    CHECK_NOTHROW(W{w1});
    CHECK_THROWS(W{w1});
  }

  SUBCASE("move construction") {
    using W = Widget<Unique<>, ThrowOnNth<Op::MoveConstruction, 3>, Value<int>>;
    W w1{};
    CHECK_NOTHROW(W{std::move(w1)});
    CHECK_NOTHROW(W{std::move(w1)});
    CHECK_THROWS(W{std::move(w1)});
  }

  SUBCASE("copy assignment") {
    using W = Widget<Unique<>, ThrowOnNth<Op::CopyAssignment, 3>, Value<int>>;
    W w1{};
    W w2{};
    CHECK_NOTHROW(w1 = w2);
    CHECK_NOTHROW(w1 = w2);
    CHECK_THROWS(w1 = w2);
  }
  SUBCASE("move assignment") {
    using W = Widget<Unique<>, ThrowOnNth<Op::MoveAssignment, 3>, Value<int>>;
    W w{};
    W w1{};
    W w2{};
    W w3{};
    CHECK_NOTHROW(w = std::move(w1));
    CHECK_NOTHROW(w = std::move(w2));
    CHECK_THROWS(w = std::move(w3));
  }

}
