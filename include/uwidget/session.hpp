#pragma once
#include <cstddef>
#include <cstdint>
#include <array>
namespace uwidget {

enum class RegisteredData: uint8_t {
  ActiveInstances = 0,
  Destructions,
  DefaultConstructions,
  CopyConstructions,
  CopyAssignments,
  MoveConstructions,
  MoveAssignments,
  _COUNT
};

template <typename Widget>
class Session {

public:
  static Session& instance() {
    static thread_local Session session{};
    return session;
  }

  void reset() {
    auto& s = Session::instance().data_;
    s.data_.fill(0);
  }

  template <RegisteredData data>
  void reg() {
    data_[static_cast<uint8_t>(data)]++;
  }

  template <RegisteredData data>
  void dereg() {
    data_[static_cast<uint8_t>(data)]--;
  }

  template <RegisteredData data>
  static size_t get_data() {
    return instance().data_[static_cast<uint8_t>(data)];
  }

private:

  std::array<size_t, static_cast<uint8_t>(RegisteredData::_COUNT)> data_{};

};
}
