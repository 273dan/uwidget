#pragma once

#include <stdexcept>

namespace uwidget {
class WidgetException : public std::runtime_error {
  public:
    explicit WidgetException(const char* msg) : runtime_error{msg} {};
  };

}
