#include "uwidget/uwidget.hpp"
using namespace uwidget;
using namespace uwidget::policy;
using namespace uwidget::detail;

using NoMove_w = Widget<NoMove>;

static_assert(NoMove_w::w_has_policy_v<Disable<Op::MoveConstruction>>, "NoMove policy expansion should contain Disable<MoveConstruction>");
static_assert(NoMove_w::w_has_policy_v<Disable<Op::MoveAssignment>>, "NoMove policy expansion should contain Disable<MoveAssignment>");
