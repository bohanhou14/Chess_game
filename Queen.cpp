// Richard Zhang, rzhang89
// Abe Hou, bhou4
// Eric Wang, ewang42
#include "Queen.h"

namespace Chess
{
  bool Queen::legal_move_shape(const Position& start, const Position& end) const {
    if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.first < '1') { //return false if the position is out of the board
      return false;
    }
    int hor_shift = getAbsolute(start.first - end.first); //record the horizontal shift
    int ver_shift = getAbsolute(start.second - end.second); //record the vertical shift

    if (hor_shift == ver_shift) { //if it is acting like a bishop
      return true;
    } else {
        if (hor_shift == 0 || ver_shift == 0) {
            return true; // if it acts as a rook(only shift in 1 direction)
        }
    }
    return false; //if it acts neither as a bishop nor as a rook, its move is invalid
  }
}
