#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using TestWidget = Widget<policy::ThrowOnCopy>;
  using TestSession = Session<TestWidget>;

  TestWidget w1{};

  bool caught = false;

  assert(TestSession::get_data<RegisteredData::ActiveInstances>() == 1);
  try {
    TestWidget w2 = w1;
  } catch (const WidgetException& e) {
    caught = true;
  }
  assert(caught == true);
  assert(TestSession::get_data<RegisteredData::ActiveInstances>() == 1);

}
