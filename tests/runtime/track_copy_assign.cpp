#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using DefaultWidget = Widget<>;
  using CurrentSession = Session<Widget<>>;
  Widget<> w{};
  Widget<> w1{};
  Widget<> w2{};

  w1 = w;
  w2 = w;

  assert(CurrentSession::get_data<RegisteredData::CopyAssignments>() == 2);
}
