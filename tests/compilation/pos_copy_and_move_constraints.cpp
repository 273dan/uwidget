#include "uwidget/uwidget.hpp"
#include <type_traits>

using namespace uwidget;
using NoCopy_w = Widget<policy::NoCopy>;
using NoMove_w = Widget<policy::NoMove>;

static_assert(!std::is_copy_assignable_v<NoCopy_w>, "No copy widget should not be copy assignable");
static_assert(!std::is_copy_constructible_v<NoCopy_w>, "No copy widget should not be copy constructable");
static_assert(!std::is_move_assignable_v<NoMove_w>, "No move widget should not be move assignable");
static_assert(!std::is_move_constructible_v<NoMove_w>, "No move widget should not be move constructable");
