#include "uwidget/uwidget.hpp"
#include <type_traits>

using namespace uwidget;
using NoCopyWidget = Widget<policy::NoCopy>;
using NoMoveWidget = Widget<policy::NoMove>;

static_assert(!std::is_copy_assignable_v<NoCopyWidget>, "No copy widget should not be copy assignable");
static_assert(!std::is_copy_constructible_v<NoCopyWidget>, "No copy widget should not be copy constructable");
static_assert(!std::is_move_assignable_v<NoMoveWidget>, "No move widget should not be move assignable");
static_assert(!std::is_move_constructible_v<NoMoveWidget>, "No move widget should not be move constructable");
