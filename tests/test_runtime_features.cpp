#include "uwidget/uwidget.hpp"
#include "doctest/doctest.h"
#include <latch>
#include <string>
#include <thread>
#include <unordered_set>

using namespace uwidget;
using namespace uwidget::policy;

TEST_CASE("value is preserved across different construction types") {
  SUBCASE("primitive value type") {
    using W = Widget<Value<int>, Unique<>>;

    W w{10};
    SUBCASE("value construction") {
      CHECK(w.value == 10);
    }
    SUBCASE("copy construction") {
      W w1{w};
      CHECK(w1.value == 10);
    }
    SUBCASE("move construction") {
      W w1{std::move(w)};
      CHECK(w1.value == 10);
    }
    SUBCASE("copy assignment") {
      W w1{};
      w1 = w;
      CHECK(w1.value == 10);
    }
    SUBCASE("move assignment") {
      W w1{};
      w1 = std::move(w);
      CHECK(w1.value == 10);
    }
  }

  SUBCASE("complex value type") {
    using W = Widget<Value<std::string>, Unique<>>;
    W w{"hello, world!"};
    SUBCASE("value construction") {
      CHECK(w.value == "hello, world!");
    }
    SUBCASE("copy construction") {
      W w1{w};
      CHECK(w1.value == "hello, world!");
    }
    SUBCASE("move construction") {
      W w1{std::move(w)};
      CHECK(w1.value == "hello, world!");
    }
    SUBCASE("copy assignment") {
      W w1{};
      w1 = w;
      CHECK(w1.value == "hello, world!");
    }
    SUBCASE("move assignment") {
      W w1{};
      w1 = std::move(w);
      CHECK(w1.value == "hello, world!");
    }

  }

}

TEST_CASE("tracking works across multiple threads") {
  using W = uwidget::Widget<Value<int>, Unique<>>;

  std::latch sync{2};

  W w0{};
  W wr{2};
  {
    std::jthread t1([&] {
        sync.arrive_and_wait();

        W w1{};
        W w2{w0};
        W w3{std::move(wr)};
    });

    std::jthread t2([&] {
        sync.arrive_and_wait();

        W w4{w0};
        w4 = w0;
    });
  }
  CHECK(W::default_constructions() == 2);
  CHECK(W::value_constructions() == 1);
  CHECK(W::copy_constructions() == 2);
  CHECK(W::copy_assignments() == 1);
  CHECK(W::move_constructions() == 1);


}

TEST_CASE("hashing") {
  using W = Widget<Value<int>, Unique<>>;

  std::unordered_set<W> us{};

  us.insert(W{1});
  us.insert(W{2});

  CHECK(us.find(W{1}) != us.end());
  CHECK(us.find(W{2}) != us.end());
  CHECK(us.find(W{42}) == us.end());
}
TEST_CASE("comparators") {
  using W = Widget<Value<int>, Unique<>>;
  W w1{1};
  W w2{2};
  W w3{42};
  W w4{42};

  CHECK(w2 > w1);
  CHECK(w1 < w2);
  CHECK(w2 >= w1);
  CHECK(w1 <= w2);
  CHECK(w1 != w2);

  CHECK(w3 == w4);

}
TEST_CASE("widgets with unique track separately") {
  using W1 = Widget<Value<float>, Unique<>>;
  using W2 = Widget<Value<float>, Unique<>>;
  using W3 = Widget<Value<float>, Unique<>>;

  W1 w_1{};

  W2 w2_1{};
  W2 w2_2{};

  W3 w3_1{};
  W3 w3_2{};
  W3 w3_3{};

  CHECK(W1::default_constructions() == 1);
  CHECK(W2::default_constructions() == 2);
  CHECK(W3::default_constructions() == 3);

}
