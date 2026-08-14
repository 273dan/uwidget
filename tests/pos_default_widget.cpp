#include "uwidget/uwidget.hpp"
#include <type_traits>

using namespace uwidget;

static_assert(std::is_copy_constructible_v<Widget<>>, "Default widget should be copy constructable");
static_assert(std::is_copy_assignable_v<Widget<>>, "Default widget should be copy assignable");
static_assert(std::is_move_constructible_v<Widget<>>, "Default widget should be move constructible");
static_assert(std::is_move_assignable_v<Widget<>>, "Default widget should be move assignable");
