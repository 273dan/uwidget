#include "uwidget/uwidget.hpp"
#include <cassert>
#include <thread>
#include <latch>

using namespace uwidget;
using namespace uwidget::policy;

int main() {
  using Default_w = uwidget::Widget<Value<int>>;

  std::latch sync{2};

  Default_w w0{};
  Default_w wr{2};
  {
    std::jthread t1([&] {
        sync.arrive_and_wait();

        Default_w w1{};
        Default_w w2{w0};
        Default_w w3{std::move(wr)};
    });

    std::jthread t2([&] {
        sync.arrive_and_wait();

        Default_w w4{w0};
        w4 = w0;
    });
  }
  assert(Default_w::default_constructions() == 2);
  assert(Default_w::value_constructions() == 1);
  assert(Default_w::copy_constructions() == 2);
  assert(Default_w::copy_assignments() == 1);
  assert(Default_w::move_constructions() == 1);


}
