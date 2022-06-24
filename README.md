## Chess_game
This project is a C++ implementation of the classic chess game for CS220: Intermediate Programming. I worked in a team of three and contributed 40% of code development, 20% of testing, and 70% of documentations. We received ~97% on this project.

## Features
This program allows two human players to play chess. The rules are the same as the standard chess rules except that it doesn't support castling.

Once the program is running, a chess game using the default chess board is created, and the user is presented with a list of possible actions as follows:

- ? — display the list of actions
- Q — quit the game
- L <filename> — load a game from the specified file
- S <filename> — save the current game to the specified file
- M <move> — try to make the specified move, where <move> is a four-character string giving the column (‘A’-‘H’) (must be an upper case to be valid!) and row (‘1’-‘8’) of the start position, followed by the column and row of the end position
- Prior to the user selecting an action, the current state of the board is presented to the user on standard output. The user can repeatedly enter one of the above action specifiers until the program ends, which happens when the game reaches checkmate or stalemate, or the user elects to quit.

## Demo
  ![image](https://user-images.githubusercontent.com/64535834/175611272-2b8a74bc-ad7b-4aad-a536-8ec79c193f91.PNG)

## Design
- UML:
  
  ![image](https://user-images.githubusercontent.com/64535834/175510247-22018c83-a033-4fb8-9c93-c909121c3cf3.PNG)
  
We implemented `point_value` by calling a function `points` in `Board.cpp` with a boolean parameter `white`(meaning the color of the piece).
In this `points` function, we have an int `count` that stores the total points and we iterate through all existing pieces. 
If this piece's color is the same as `white`, we increment `count` by the `point_value()` of this piece. 
For each piece, we implemented its unique `point_value()` that returns the point worth of this piece. 

For basic operation of the game `move`, we first check whether the start and end positions are valid. We check if it is a pair of valid capture/move positions and if they are within the bound of the board.
 
Then, we check if the path is clear (i. e., if the start and end position are on the same line). If all tests above passed, 
we check if the move will result in a checked position (by creating a copy of the game and calling `move`). If so, we throw an exception. If all tests are passed and this is indeed a valid move, we check if this move results in a promotion.

We also implemented a function called `can_move`. It goes over every piece, and for each piece, creates 
a vector of valid positions it can go to. Then it will try these positions one by one 
and see if they result an `in_check`. If any of them doesn't result in check, `can_move` will return true because there is 
at least one valid move possible and the game continues. 
  
This `can_move` method is also used to test `stalemate` and `checkmate`: if a player is in check and `can_move` returns false, this is a `checkmate`. Similarly, if a player is not in check but `can_move` returns false, this is a `stalemate`.

To save the game, we go over every position on the game board and print the appropriate representation of this block to 
the file (like K for white king and - for empty space). Then we check the current color and place it to the bottom of the file. When 
loading, we read the file line by line. If a line includes invalid characters 
or is too long/too short, we throw an exception that says `cannot load game`.
