// Richard Zhang, rzhang89
// Abe Hou, bhou4
// Eric Wang, ewang42
#include "Knight.h"

namespace Chess
{
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {
    if (end.first > 'H' || end.first < 'A' || end.second > '8' || end.first < '1') { //return false if the position is out of the board
      return false;
    }
    int hor_shift = getAbsolute(start.first - end.first); //record the horizontal shift
    int ver_shift = getAbsolute(start.second - end.second); //record the vertical shift
    if(hor_shift == 2) {//move to right/left by 2
        return ver_shift == 1; //vertical shift must be equal to 1
    } else if (hor_shift == 1) { //move to right/left by 1
        return ver_shift == 2; //vertical shift must be equal to 2
    } else {
        return false; //a knight must move either 2 or 1 step horizontally
    }
  }


}
