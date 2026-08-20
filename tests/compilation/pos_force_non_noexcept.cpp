#include "uwidget/uwidget.hpp"
using namespace uwidget;
using namespace uwidget::policy;
using namespace uwidget::detail;

using ForceNonNoexceptMoveAssign_w = Widget<ForceNonNoexcept<Op::MoveAssignment>>;

static_assert(
    !std::is_nothrow_move_assignable_v<ForceNonNoexceptMoveAssign_w>,
    "ForceNonNoexcept<MoveAssignment> should make Widget non-nothrow move assignable"
);
