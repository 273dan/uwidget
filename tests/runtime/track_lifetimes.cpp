#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using CurrentSession = Session<Widget<>>;

  assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 0);
  {
    Widget<> w1{};
    Widget<> w2{};
    assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 2);
    
    {
      Widget<> w3 = w1;
      assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 3);
    }

    assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 2);
    assert(CurrentSession::get_data<RegisteredData::Destructions>() == 1);

    {
      Widget<> w3 = std::move(w1);
      assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 3);
    }

    assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 2);
    assert(CurrentSession::get_data<RegisteredData::Destructions>() == 2);
    
  }

  assert(CurrentSession::get_data<RegisteredData::ActiveInstances>() == 0);
  assert(CurrentSession::get_data<RegisteredData::Destructions>() == 4);




}
