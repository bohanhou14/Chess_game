// Richard Zhang, rzhang89
// Abe Hou, bhou4
// Eric Wang, ewang42
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "Piece.h"
#include "Board.h"
#include "Exceptions.h"

namespace Chess
{

	class Game {

	public:
		// This default constructor initializes a board with the standard
		// piece positions, and sets the state to white's turn
		Game();
		~Game();

		// Returns true if it is white's turn
		/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		bool turn_white() const { return is_white_turn; }
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    // Displays the game by printing it to stdout
		void display() const { board.display(); }
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    // Checks if the game is valid
		bool is_valid_game() const { return board.has_valid_kings(); }

		// Attempts to make a move. If successful, the move is made and
		// the turn is switched white <-> black. Otherwise, an exception is thrown
		void make_move(const Position& start, const Position& end);
		// Check if the move to make is allowed and throws relevant exceptions
		void make_move_exception_check(const Position& start, const Position& end) const;
		// Returns true if the designated player is in check
		bool in_check(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_mate(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_stalemate(const bool& white) const;

        // Return the total material point value of the designated player
        int point_value(const bool& white) const;
		// Update the current turn of the game
		void next_turn() { is_white_turn = is_white_turn ? false: true;}

		// Return the valid moves of the piece in this position(not checking the check cases)
		std::vector<Position> valid_moves(Position p) const;

		// Check if the valids moves includes at least one move that will not result in check
		bool non_check_moves(std::vector<Position> v,Position start,Game game) const;

		// Checks the whole board and see if any piece can move anywhere(that doesn't result in check)
		bool can_move(Game game) const;
		// Copy constructor
		Game(const Game& o):board(o.board), is_white_turn(o.is_white_turn){};
		
		//move a piece from one place to another(without changing turn)
		void move(const Position& start, const Position& end);

	private:
		// The board
		Board board;

		// Is it white's turn?
		bool is_white_turn;

        // Writes the board out to a stream
        friend std::ostream& operator<< (std::ostream& os, const Game& game);

        // Reads the board in from a stream
        friend std::istream& operator>> (std::istream& is, Game& game);
	};
}
#endif // GAME_H
