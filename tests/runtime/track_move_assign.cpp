#include "uwidget/uwidget.hpp"
#include <cassert>

using namespace uwidget;

int main() {
  using DefaultWidget = Widget<>;
  using CurrentSession = Session<Widget<>>;

  Widget<> w_move_from1{};
  Widget<> w_move_from2{};
  Widget<> w1{};
  Widget<> w2{};
  w1 = std::move(w_move_from1);
  w2 = std::move(w_move_from2);


  assert(CurrentSession::get_data<RegisteredData::MoveAssignments>() == 2);
}
