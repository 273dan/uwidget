#include "doctest/doctest.h"
#include "uwidget/uwidget.hpp"

using namespace uwidget;
using namespace uwidget::policy;

TEST_CASE("track metrics") {

  using W = Widget<Unique<>, Value<int>>;

  W::reset_metrics();

  W w1{};
  W w2{};
  W w3{};

  SUBCASE("default construction") {
    CHECK(W::default_constructions() == 3);
    CHECK(W::value_constructions() == 0);
    CHECK(W::copy_constructions() == 0);
    CHECK(W::move_constructions() == 0);
    CHECK(W::copy_assignments() == 0);
    CHECK(W::move_assignments() == 0);
  }

  SUBCASE("value construction") {
    W w4{10};
    W w5{20};
    CHECK(W::default_constructions() == 3);
    CHECK(W::value_constructions() == 2);
    CHECK(W::copy_constructions() == 0);
    CHECK(W::move_constructions() == 0);
    CHECK(W::copy_assignments() == 0);
    CHECK(W::move_assignments() == 0);
  }

  SUBCASE("copy construction") {
    W w4{ w1 };
    W w5{ w1 };
    CHECK(W::default_constructions() == 3);
    CHECK(W::value_constructions() == 0);
    CHECK(W::copy_constructions() == 2);
    CHECK(W::move_constructions() == 0);
    CHECK(W::copy_assignments() == 0);
    CHECK(W::move_assignments() == 0);
  }

  SUBCASE("move construction") {
    W w3{ std::move(w2) };
    W w4{ std::move(w3) };
    CHECK(W::default_constructions() == 3);
    CHECK(W::value_constructions() == 0);
    CHECK(W::copy_constructions() == 0);
    CHECK(W::move_constructions() == 2);
    CHECK(W::copy_assignments() == 0);
    CHECK(W::move_assignments() == 0);
  }

  SUBCASE("copy assignment") {
    w1 = w2;
    w2 = w3;
    CHECK(W::default_constructions() == 3);
    CHECK(W::value_constructions() == 0);
    CHECK(W::copy_constructions() == 0);
    CHECK(W::move_constructions() == 0);
    CHECK(W::copy_assignments() == 2);
    CHECK(W::move_assignments() == 0);
  }

  SUBCASE("move assignment") {
    w1 = std::move(w2);
    w2 = std::move(w3);
    CHECK(W::default_constructions() == 3);
    CHECK(W::value_constructions() == 0);
    CHECK(W::copy_constructions() == 0);
    CHECK(W::move_constructions() == 0);
    CHECK(W::copy_assignments() == 0);
    CHECK(W::move_assignments() == 2);
  }
  SUBCASE("active instances") {
    CHECK(W::active_instances() == 3);
    {
      W w4{};
      W w5{};
      CHECK(W::active_instances() == 5);
      {
        W w6{};
        CHECK(W::active_instances() == 6);
      }
      CHECK(W::active_instances() == 5);

    }
    CHECK(W::active_instances() == 3);
  }
}
