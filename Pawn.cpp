// Richard Zhang, rzhang89
// Abe Hou, bhou4
// Eric Wang, ewang42
#include "Pawn.h"

namespace Chess
{
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
    if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.first < '1') { //return false if the position is out of the board
      return false;
    }
    int hor_shift = getAbsolute(start.first - end.first); //record the horizontal shift
    int ver_shift = start.second - end.second; //record the vertical shift

    if(hor_shift != 0) {
        return false; //pawns cannot move horizontally
    }

    if(is_white()) { //if this is a black piece
        if (ver_shift == -1) {
            return true; //moving one step is always allowed
        }
        if (ver_shift == -2 && start.second == '2') {
            return true; //can move two steps in the first move
        }
        return false; //else return false
    } else { //if this is a white piece
        if (ver_shift == 1) {
            return true; //moving one step is always allowed
        }
        if (ver_shift == 2 && start.second == '7') {
            return true; //can move two steps in the first move
        }
        return false; //else return false
    }
  }

  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const {
    if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.first < '1') { //return false if the position is out of the board
      return false;
    }
    int hor_shift = getAbsolute(start.first - end.first); //record the horizontal shift
    int ver_shift = start.second - end.second; //record the vertical shift

    if (hor_shift != 1) {
        return false; //pawn can only capture pieces in its left/right row
    }
    if (is_white() && ver_shift == -1) {
        return true; //move one step virtically(upward)
    }
    if (!is_white() && ver_shift == 1) {
        return true; //move one step virtically(downward)
    }
    return false;
  }

}
