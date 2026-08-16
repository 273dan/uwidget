#include "uwidget/uwidget.hpp"
#include <type_traits>

using namespace uwidget;
using namespace uwidget::policy;

using ThrowOnNthMove_w = Widget<ThrowOnNthOperation<Op::MoveConstruction, 3>>;

static_assert(!std::is_nothrow_move_constructible_v<ThrowOnNthMove_w>, "ThrowOnNthMove widget should not be nothrow move constructible");

