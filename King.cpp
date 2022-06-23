// Richard Zhang, rzhang89
// Abe Hou, bhou4
// Eric Wang, ewang42
#include "King.h"

namespace Chess
{
  bool King::legal_move_shape(const Position& start, const Position& end) const {
    if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.first < '1') { //return false if the position is out of the board
      return false;
    }
    int hor_shift = getAbsolute(start.first - end.first); //record the horizontal shift
    int ver_shift = getAbsolute(start.second - end.second); //record the vertical shift
    if (hor_shift <= 1 && ver_shift <= 1) { //if the horizontal position and vertical position shifted for <= positions
      return true;
    } else {
      return false;
    }
  }

}
