// Richard Zhang, rzhang89
// Abe Hou, bhou4
// Eric Wang, ewang42
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"


namespace Chess
{
	//Check if the position is within the boundary of the chessboard
	bool in_bound(const Position& pos);

  class Board {

		// Throughout, we will be accessing board positions using Position defined in Piece.h.
		// The assumption is that the first value is the column with values in
		// {'A','B','C','D','E','F','G','H'} (all caps)
		// and the second is the row, with values in {'1','2','3','4','5','6','7','8'}

	public:
		// Default constructor
		Board();
		~Board();

		// Returns a const pointer to the piece at a prescribed location if it exists,
		// or nullptr if there is nothing there.
		const Piece* operator() (const Position& position) const;

		// Attempts to add a new piece with the specified designator, at the given position.
		// Throw exception for the following cases:
		// -- the designator is invalid, throw exception with error message "invalid designator"
		// -- the specified position is not on the board, throw exception with error message "invalid position"
		// -- if the specified position is occupied, throw exception with error message "position is occupied"
		void add_piece(const Position& position, const char& piece_designator);
		// Copy constructor 
		Board(const Board &o);
		// Displays the board by printing it to stdout
		void display() const;
		// Returns true if the board has the right number of kings on it
		bool has_valid_kings() const;
		// Returns the current point of one side
		int points(bool white) const;
		// Check if the piece belongs to the player who is supposed to make moves
		bool check_match(const Position& pos, bool is_white_turn) const;
		// Check if the piece is moving along a legal shape
		bool has_legal_move_shape(const Position& start, const Position& end) const;
		// Check if the piece has a legal capture shape
		bool has_legal_capture_shape(const Position& start, const Position& end)const;
		// Check if the path is clear
		bool is_path_clear(const Position& start, const Position& end)const;
		// Check if the piece is going to move or to caputre
		bool is_to_move(const Position& end)const;
		// Return the position of the piece queried
		const Position get_piece_position(const char& piece_designator) const;
		// Move a piece and update the sparse map storing pieces and positions
		void move_piece(const Position& start, const Position& end);
		// Capture a piece and update the sparse map storing pieces and positions
		void capture_piece(const Position& start, const Position& end);
		// Check if the given side is in check
		bool in_check(const bool& white) const;
		// Remove a piece in a position
		void remove_piece(const Position& pos);
		
		
	private:
		// The sparse map storing the pieces, keyed off locations
		std::map<Position, Piece*> occ;
		// Helper method for the destructor
		void clear();
        // Write the board state to an output stream
		friend std::istream& operator>> (std::istream& is, Board& board);
        friend std::ostream& operator<< (std::ostream& os, const Board& board);
		
	};
	
}
#endif // BOARD_H
