// Richard Zhang, rzhang89
// Abe Hou, bhou4
// Eric Wang, ewang42
#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"
#include<vector>

using std::string;
using std::vector;
using std::endl;
using std::cout;
namespace Chess
{
  Board::Board(){}
  Board::~Board(){
    clear();
  }

  //Check if the position is within the boundary of the chessboard
 bool in_bound(const Position& pos){
    if(pos.first > 'H' || pos.first < 'A' || pos.second > '8' || pos.first < '1') { //check invalid position
        return false;
    }
    return true;
  }

  // Return the piece given a position
  const Piece* Board::operator()(const Position& position) const { //! this may be useful in other files ——R
    return occ.find(position) != occ.cend() ? occ.find(position)->second : nullptr;
  }
  // Return the position given a piece
  const Position Board::get_piece_position(const char& piece_designator) const { //! don't call it often, it's relatively slow ——R
    for(std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.cbegin();it != occ.cend();it++) {
      if(it->second->to_ascii() == piece_designator) {
        return it->first;
      }
    }
    return Position({'0','0'}); //nothing is found
  }
  
  // Check if the piece to be moved has a consistent color with the player
  bool Board::check_match(const Position& pos, bool is_white_turn) const{
    if(is_white_turn) {
      if((*this)(pos)->is_white()){
        return true;
      }
    }
    if(!is_white_turn) {
      if((*this)(pos)->is_white() == false) {
        return true;
      }
    }

    return false;
  }

  //Check if the move shape is legal
  bool Board::has_legal_move_shape( const Position& start, const Position& end) const {
    if ((*this)(end)!=nullptr) {
      return true;
    }
    if((*this)(start)->legal_move_shape(start, end)) {
      return true;
    }
    return false;
  }
  //Check if the capture shape is legal
  bool Board::has_legal_capture_shape(const Position& start, const Position& end)const {
    if((*this)(start)->legal_capture_shape(start,end)){
      return true;
    }
    return false;
  }

  //Check if the path is clear
  bool Board::is_path_clear(const Position & start, const Position& end)const {
    int hor_shift = ((start.first - end.first)>=0)?(start.first - end.first):(end.first - start.first); //record the horizontal shift
    int ver_shift = ((start.second - end.second)>=0)?(start.second - end.second):(end.second - start.second); //record the vertical shift
    
    //this section checks if the path is clear. note that only moving in a straight line will be checked
    //because only the knight and the mystery piece moves in a nonstraight line and won't be blocked.
    if (hor_shift == 0) {//meaning it moves vertically
      if (end.second>start.second) {//meaning moving upward
        for (int i = 1; i < ver_shift; i++) {
          if ((*this)(Position({start.first, (char)(start.second +i)}))) {//if there is a non-nullptr
            return false;
          }
        }
      }

      if (end.second<start.second) {//meaning moving downward
        for (int i = 1; i < ver_shift; i++) {
          if ((*this)(Position({start.first, (char)(start.second-i)}))) {
            return false;
          }
        }
      }
    }

    else if (ver_shift == 0) {//moving horizontally
      if (end.first>start.first) {//meaning moving rightward
        for (int i = 1; i < hor_shift; i++) {
          if ((*this)(Position({(char)(start.first+i), start.second}))) {
            return false;
          }
        }
      }
      if (end.first<start.first) {//meaning moving leftward
        for (int i = 1; i < hor_shift; i++) {
          if ((*this)(Position({(char)(start.first-i), start.second}))) {
            return false;
          }
        }
      }
    }

    else if (ver_shift == hor_shift && ver_shift > 0) {//moving diagnoally
      if (start.first < end.first && start.second < end.second) {//right-upward
        for (int i = 1; i < hor_shift; i++) {
          if ((*this)(Position({(char)(start.first+i), (char)(start.second+i)}))) {
            return false;
          }
        }
      }
      if (start.first < end.first && start.second > end.second) {//right-downward
        for (int i = 1; i < hor_shift; i++) {
          if ((*this)(Position({(char)(start.first+i), (char)(start.second-i)}))) {
            return false;
          }
        }
      }
      if (start.first > end.first && start.second < end.second) {//left-upward
        for (int i = 1; i < hor_shift; i++) {
          if ((*this)(Position({(char)(start.first-i), (char)(start.second+i)}))) {
            return false;
          }
        }
      }
      if (start.first > end.first && start.second > end.second) {//left-downward
        for (int i = 1; i < hor_shift; i++) {
          if ((*this)(Position({(char)(start.first-i), (char)(start.second-i)}))) {
            return false;
          }
        }
      }
    }
    //if reached here, meaning all postition on the paths are checked to have no pre-existing pieces
    return true;
  }
  
  //Move a piece to an empty spot
  void Board::move_piece(const Position& start, const Position& end) {
    std::map<Position, Piece*>::iterator it = occ.find(start);
    //insert the updated piece
    occ.insert(std::pair<Position, Piece*>(end, it->second));
    //remove the old piece
    occ.erase(start);
  }
  //Copy constructor
  Board::Board(const Board & o) {
    for (std::map<Position, Piece*>::const_iterator it = o.occ.cbegin();
    it != o.occ.cend(); ++it) {
      add_piece(it->first, it->second->to_ascii());
      }
    }

  //Remove the captured piece
  void Board::capture_piece(const Position& start, const Position& end) {
    std::map<Position, Piece*>::iterator it = occ.find(start);
    //remove the end piece(it is being captured)
    remove_piece(end);
    //insert the updated piece
    occ.insert(std::pair<Position, Piece*>(end, it->second));
    //remove the old piece
    occ.erase(start);
  }
  //Remove a given piece
  void Board::remove_piece(const Position& pos) {
    delete occ.find(pos)->second; //clear it from memory
    occ.erase(pos);
  }
  // Check if the end position is empty
  bool Board::is_to_move(const Position& end) const{
    if((*this)(end)==nullptr){//meaning it is to move
      return true;
    }
    return false;
  }


  void Board::add_piece(const Position& position, const char& piece_designator) {
    if((*this)(position) != nullptr) {
      throw Exception("position is occupied");
    }
    if(!in_bound(position)){
      throw Exception("invalid position");
    }
    Piece* new_piece = create_piece(piece_designator);
    if(new_piece == nullptr) { //when this new piece is invalid
      throw Exception("invalid designator");
    }
    occ[position] = new_piece;
  }

  void Board::display() const {
    bool white = false; //represent that the current line starts with a white block
    std::cout << " ABCDEFGH" << std::endl; //print the top label
    for(char i = '8';i >= '1';i--){ //print out each line(from 8 to 1 because 8 is on top)
      std::cout << i; //print out the label for this line
      white = !white;
      for(char j = 'A';j <= 'H';j++){ //set each element in this row
        if(white == true){ //set the colors accordingly
          Terminal::color_all(false, Terminal::BLACK, Terminal::WHITE);
        } else {
          Terminal::color_all(false, Terminal::WHITE, Terminal::BLACK);
        }
        Position p = std::make_pair(j,i); //get this current position     
        if((*this)(p) != nullptr){//print out this block
          std::cout << ((*this)(p))->to_ascii();
        } else {
          std::cout << " ";
        }
        white = !white; //change white so that the next block will be of different color
      }
      Terminal::set_default(); //the labels should be print in regular colors
      std::cout << std::endl; //go to the next line
    }
  }

  int Board::points(bool white) const {
    int count = 0; //count the total score
    for(std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.cbegin();it != occ.cend();it++) {
			if (it->second->is_white() == white) {
        count += it -> second -> point_value();
      }
		}
    return count;
  }

  bool Board::has_valid_kings() const { 
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	 it != occ.end();
	 it++) {
      if (it->second) {
	switch (it->second->to_ascii()) {
	case 'K':
	  white_king_count++;
	  break;
	case 'k':
	  black_king_count++;
	  break;
	}
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  void Board::clear() { //clear everything in board and deallocate space(written by us)
    for (std::map<std::pair<char, char>, Piece*>::iterator it = occ.begin();it != occ.end();it++) {
      delete it->second; //deallocates the piece
    }
    occ.clear(); //cleans the map
  }

  std::istream& operator>> (std::istream& is, Board& board) {
    board.clear();
    char c; //store the current char
    for(char i = '8'; i >= '1'; i--) { //every row
      std::string str; //this current line
      getline(is, str);
      if(str.length() != 8) {
        throw Exception("loading error: not having exactly 8 elements in one row");
      }
      for(int j = 0; j < 8;j++){ //every element in row
        c = str[j];
        if(c != '-'){ //if this is not an empty space,add the piece on this place to the board
          Position p = std::make_pair(j+'A',i); //remember this position
          board.add_piece(p,c);
        }
      }
    }
    return is;
  }


  	bool Board::in_check(const bool& white) const {
      //get the king's position based on the input color
      Position king = white == true? get_piece_position('K'):get_piece_position('k');

      //go over every piece to see if they are threats to the king in this position
      for(std::map<Position, Piece*>::const_iterator it = occ.cbegin();it != occ.cend();it++){
        if (white) {
          //if the searched piece is black
          if (!it->second->is_white()) {
            //if the king is directly capturable by this piece, return true
            if(it->second->legal_capture_shape(it->first, king) && is_path_clear(it->first, king)) {
              return true;
            }
          }
        } else {
          //when king is black, the searched piece is white
          //if the king is directly capturable by this piece, return true
          if (it->second->is_white()) {
            if(it->second->legal_capture_shape(it->first, king) && is_path_clear(it->first, king)) {
              return true;
            }
          }
        }
      }
      return false;
	}

  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }
}
