#pragma once

#include <exception>

namespace uwidget {
class WidgetException : std::exception {
  public:
    explicit WidgetException(const char* msg) : msg_{msg} {};
    const char* what() const noexcept override {return msg_; }
  private:
    const char* msg_;

  };

}
