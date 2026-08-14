#include "uwidget/uwidget.hpp"
#include <type_traits>
#include <string>

using namespace uwidget;


// primitive value_t
using FloatWidget = Widget<float>;
static_assert(std::is_same_v<FloatWidget::value_t, float>, "FloatWidget value should be double");
static_assert(std::is_copy_constructible_v<FloatWidget>, "FloatWidget should be copy constructible");
static_assert(std::is_move_constructible_v<FloatWidget>, "FloatWidget should be move constructible");
static_assert(std::is_copy_assignable_v<FloatWidget>, "FloatWidget should be copy assignable");
static_assert(std::is_move_assignable_v<FloatWidget>, "FloatWidget should be move assignable");
FloatWidget fw{42.0f};

// complex value_t
using StringWidget = Widget<std::string>;
static_assert(std::is_same_v<StringWidget::value_t, std::string>, "StringWidget value should be double");
static_assert(std::is_copy_constructible_v<StringWidget>, "StringWidget should be copy constructible");
static_assert(std::is_move_constructible_v<StringWidget>, "StringWidget should be move constructible");
static_assert(std::is_copy_assignable_v<StringWidget>, "StringWidget should be copy assignable");
static_assert(std::is_move_assignable_v<StringWidget>, "StringWidget should be move assignable");
StringWidget sw{"hello, world!"};

// with policies
using FloatNoCopyWidget = Widget<float, policy::NoCopy>;
static_assert(std::is_same_v<FloatNoCopyWidget::value_t, float>, "FloatNoCopyWidget value should be double");
static_assert(!std::is_copy_constructible_v<FloatNoCopyWidget>, "FloatNoCopyWidget should not be copy constructible");
static_assert(std::is_move_constructible_v<FloatNoCopyWidget>, "FloatNoCopyWidget should be move constructible");
static_assert(!std::is_copy_assignable_v<FloatNoCopyWidget>, "FloatNoCopyWidget should not be copy assignable");
static_assert(std::is_move_assignable_v<FloatNoCopyWidget>, "FloatNoCopyWidget should be move assignable");
FloatNoCopyWidget fncw{42.0f};
