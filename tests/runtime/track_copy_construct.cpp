#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using DefaultWidget = Widget<>;
  using CurrentSession = Session<Widget<>>;

  Widget<> w{};
  Widget<> w1{w};
  Widget<> w2{w};


  assert(CurrentSession::get_data<RegisteredData::CopyConstructions>() == 2);
}
