// Richard Zhang, rzhang89
// Abe Hou, bhou4
// Eric Wang, ewang42
#include "Bishop.h"

namespace Chess
{
  bool Bishop::legal_move_shape(const Position& start, const Position& end) const {
    if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.first < '1') { //return false if the position is out of the board
      return false;
    }
    int hor_shift = getAbsolute(start.first - end.first); //record the horizontal shift
    int ver_shift = getAbsolute(start.second - end.second); //record the vertical shift
    if (hor_shift != 0 && hor_shift == ver_shift) { //if the absolute value of hor_shift and ver_shift are equal(and are not zero)
      return true;
    } else {
      return false;
    }
  }
}
