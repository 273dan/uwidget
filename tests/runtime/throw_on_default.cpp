#include "uwidget/policy.hpp"
#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using TestWidget = Widget<int, policy::ThrowOnDefaultConstruction>;
  using TestSession = Session<TestWidget>;

  TestWidget w1{42};

  bool caught{false};
  try {
    TestWidget w2;
  } catch (const WidgetException& e) {
    caught = true;
  }
  assert(caught == true);
  assert(TestSession::get_data<RegisteredData::ActiveInstances>() == 1);

}
