// Richard Zhang, rzhang89
// Abe Hou, bhou4
// Eric Wang, ewang42
#include <cassert>
#include "Game.h"
#include <vector>
namespace Chess
{
	Game::Game() : is_white_turn(true) {
		// Add the pawns
		for (int i = 0; i < 8; i++) {
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
		board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

		// Add the knights
		board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
		board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

		// Add the bishops
		board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
		board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

		// Add the kings and queens
		board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
		board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
		board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
		board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
	}
	
	Game::~Game() {
		board.~Board();
	}

	//check invalid moves and throw appropriate exception
	void Game::make_move_exception_check(const Position& start, const Position& end)const {
		if(!in_bound(start)) {
			throw Exception("Could not make move: start position is not on board");
		}
		if (!in_bound(end)) {
			throw Exception("Could not make move: end position is not on board");
		}
		if(!board(start)) {
			throw Exception("Could not make move: no piece at start position");
		}
		if(!board.check_match(start, is_white_turn)){
			throw Exception("Could not make move: piece color and turn do not match");
		}
		if (start == end) {//if there's no change in position
			throw Exception("Could not make move: illegal move shape");
		}
		if (board.is_to_move(end)) {//check if the piece is going to move or capture
			if(!board.has_legal_move_shape(start, end)){
				throw Exception("Could not make move: illegal move shape");
			}
		}else {//meaning the piece is going to caputre
			if(!board.has_legal_capture_shape(start, end)){
				throw Exception("Could not make move: illegal capture shape");
			}
			//check if the piece is capturable; if the pieces matches the player's turn it is NOT capturable
			if(board.check_match(end, is_white_turn)) {
				throw Exception("Could not make move: cannot capture own piece");
			}
		}
		if(!board.is_path_clear(start, end)) {
			throw Exception("Could not make move: path is not clear");
		}

	}
	// attempt to make a move
	void Game::make_move(const Position& start, const Position& end) {
		move(start,end);
		//handle cases of promotion
		if (board(end)->to_ascii() == 'p' && end.second == '1') {
			board.remove_piece(end);
			board.add_piece(end,'q');
		} else if (board(end)->to_ascii() == 'P' && end.second == '8') {
			board.remove_piece(end);
			board.add_piece(end,'Q');
		}
		next_turn();	
	}

	//move a piece from one position to another, checking if it's possible
	void Game::move(const Position& start, const Position& end) {
		//check all the possible exceptions.
		make_move_exception_check(start, end);
		if (board.is_to_move(end)) {
			board.move_piece(start,end);
			if (in_check(is_white_turn)) {
				board.move_piece(end,start); //move the piece back(to restore the original position)
				throw Exception("Could not make move: move exposes check");
			}
		} else { //if we are trying to capture a piece
			char code = board(end)->to_ascii(); //save the piece in the end position
			board.capture_piece(start,end); //capture it
			if (in_check(is_white_turn)) {
				//if this capture results in checking yourself, put everything back
				board.move_piece(end,start);
				board.add_piece(end,code);
				throw Exception("Could not make move: move exposes check");
			}
		}
	}

	bool Game::in_check(const bool& white) const {
		return board.in_check(white);
	}

	
	//returns a vector that contains all possible moves of a piece(excluding the ones that will result in check)
	std::vector<Position> Game::valid_moves(Position p) const{
		std::vector<Position> moves;
		for(char i = '8'; i >= '1'; i--) { //every row
			for(char j = 'A'; j <= 'H';j++){ //every element in row
				try{
					make_move_exception_check(p,std::make_pair(j,i));
					moves.push_back(std::make_pair(j,i)); //if the check is passed, this point is a valid move
				} catch (const Exception& ex) {
					continue; //if there is an exception caught in this phase, that means that this point is not a valid move
				}
			}
    }
		return moves;
	}

	bool Game::non_check_moves(std::vector<Position> v,Position start,Game game) const {
		for (std::vector<Position>::const_iterator it = v.cbegin();it!=v.cend();it++) {
			if(game.board.is_to_move(*it)) { //if this action is to move
				try {
					game.move(start,*it); //if we successfully moved this piece(without resulting in check)
					return true; //this piece has a valid move
				} catch (const Exception& ex) {
					continue;
				}
			} else {
				try {
				  //char code = board(*it)->to_ascii(); //store the piece that will be captured
					game.move(start,*it); //if we successfully makes this capture
					return true; //show that this piece has a valid action(capture)
				} catch (const Exception& ex) {
					continue;
				}
			}
		}
		return false; //if no valid move is found, return false
	}

	bool Game::can_move(Game game) const{
		for (char i = '8'; i >= '1'; i--) {//go over every position
			for (char j = 'A'; j <= 'H'; j++) {
				Position p = std::make_pair(j,i);
				if (board(p) == nullptr || board(p)->is_white()!=is_white_turn) {
					continue; //if no piece is in this position or if the color is different, skip this position
				}
				std::vector<Position> v = valid_moves(p); //get the valid moves of piece in this position
				if (non_check_moves(v,p,game)) {
					return true; //if we find a valid move, that means we still can move, so return true
				}
			}
		}
		return false; //no valid move is found, return false
	}
	


	bool Game::in_mate(const bool& white) const {
		Game g (*this);
		//impossible to be in check mate if you are not in check, so no need to waste time
		if (in_check(white) && !can_move(g)) {
			return true; //if this piece is in check but there is no move that can get it out of this position, return true
		}
		return false;
	}


	bool Game::in_stalemate(const bool& white) const {
		Game g (*this);
		//impossible to be in stalemate if you are in check, so no need to waste time
		if (!in_check(white) && !can_move(g)) {
			return true; //if this piece is in check but there is no move that can get it out of this position, return true
		}
		return false;
	}

    // Return the total material point value of the designated player
    int Game::point_value(const bool& white) const { //written by us, may have bugs(but it seems to be working)
		return board.points(white);
    }


    std::istream& operator>> (std::istream& is, Game& game) {
		if(!is){
			throw Exception("loading error: file does not exist");
		}
		is >> game.board; //load every piece to the board
		char c;
		is.get(c);
		if(c != 'w' && c != 'b') {
			throw Exception("loading error: invalid player color");
		}
		game.is_white_turn = c == 'w' ? true : false; //set the current player color
		return is;
	}

	std::ostream& operator<< (std::ostream& os, const Game& game) {
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}
}
