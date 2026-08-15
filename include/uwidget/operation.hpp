#include <cstdint>
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
}
