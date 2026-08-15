#include "uwidget/uwidget.hpp"
#include <type_traits>

using namespace uwidget;
using namespace uwidget::policy;

using Default_w = Widget<>;

static_assert(std::is_copy_constructible_v<Default_w>, "Default widget should be copy constructable");
static_assert(std::is_copy_assignable_v<Default_w>, "Default widget should be copy assignable");
static_assert(std::is_move_constructible_v<Default_w>, "Default widget should be move constructible");
static_assert(std::is_move_assignable_v<Default_w>, "Default widget should be move assignable");
static_assert(std::is_default_constructible_v<Default_w>, "Default widget should be default constructible");
