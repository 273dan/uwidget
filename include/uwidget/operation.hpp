#pragma once

#include <cstdint>
#include <string_view>

namespace uwidget {

enum class Op: uint8_t {
  ActiveInstance = 0,
  Destruction,
  DefaultConstruction,
  CopyConstruction,
  CopyAssignment,
  MoveConstruction,
  MoveAssignment,
  ValueConstruction,
  _COUNT
};

template <Op OpV> 
inline constexpr std::string_view op_message_v =
  "Unknown Operation";

template <> 
inline constexpr std::string_view op_message_v<Op::ActiveInstance> =
  "Active Instance";

template <> 
inline constexpr std::string_view op_message_v<Op::Destruction> =
  "Destruction";

template <> 
inline constexpr std::string_view op_message_v<Op::DefaultConstruction> =
  "Default Construction";

template <> 
inline constexpr std::string_view op_message_v<Op::CopyConstruction> =
  "Copy Construction";

template <> 
inline constexpr std::string_view op_message_v<Op::CopyAssignment> =
  "Copy Assignment";

template <> 
inline constexpr std::string_view op_message_v<Op::MoveConstruction> =
  "Move Construction";

template <> 
inline constexpr std::string_view op_message_v<Op::MoveAssignment> =
  "Move Assignment";

template <> 
inline constexpr std::string_view op_message_v<Op::ValueConstruction> =
  "Value Construction";


}


