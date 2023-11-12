# GIPF
## About game
GIPF is a deterministic game for two players with a zero-sum without draws. The rules of the official version are available [here](https://files.rebel.pl/files/instrukcje/Instrukcja_Gipf_PL.pdf).

## Task
The task consists of two parts:
- implementation of the engine for a generalized version of the game. Generalization introduces a list **L(S, K, GW, GB)** of parameters describing the game. This allows for easy modification of its characteristic properties, such as board size, the number of pieces required for a specific combination triggering a certain effect in the game, like capturing/recovering pieces, etc.
- implementation of a program that solves the game or certain states of the game (e.g., end states) for different values of the parameters in the list L. The fundamental solving algorithm will be the [Minimax](https://en.wikipedia.org/wiki/Minimax) with [alpha-beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning)

## About L(S, K, GW, GB)
- S - the board size expressed as the number of hexagonal board fields on each side of the board
- K the number of player pieces that trigger the collection of pieces
- GW - the number of pieces belonging to the white player
- GB - the number of pieces belonging to the black player, which can differ from GW

## Game engine
The game engine should allow:
- **Loading the game state** : The first line will contain four numbers describing the game (content of the L list). The next line will contain the number of pawns in reserves for the player playing with white and black, respectively, and a digit indicating the active player. The subsequent lines will contain information describing the state of the board. The initial board of the original version of the game will look like this:
```
4 4 15 15
12 12 W
   W _ _ B
  _ _ _ _ _
 _ _ _ _ _ _
B _ _ _ _ _ W
 _ _ _ _ _ _
  _ _ _ _ _
   W _ _ B
```
Other exemplary states:
```
2 2 5 5
2 2 W
 W B
B _ W
 W B
```
```
3 3 10 9 W
7 6 W
  W _ B
 _ _ _ _
B _ _ _ W
 _ _ _ _
  W _ B
```
- **Applying a move to the current board** described according to the schema: xN - yM, where xN is the field where a pawn from the player's reserve is placed, and yM is the field in the direction to which just placed pawn is moved. This notation is borrowed from the program [Gipf for One (GF1)](https://gf1.sourceforge.net/#info) and looks as shown in the diagram below:
  
  ![Gipf for One](https://gf1.sourceforge.net/screen1.png)

Note that neighboring dots will not be used in the board representation because they are only used during the first phase of a move when a player places a pawn from the reserves. After placing a pawn on a dot, the player must move it. If such a move would push a pawn on the opposite side onto a dot, it is not allowed. In such a case, the game engine should remember this move as invalid and should not analyze subsequent moves. When requesting to print the board state, the state preceding this invalid move will be printed. If multiple rows consisting of K (or more) pawns of the same color are formed at the same time and these rows intersect, the command must contain information about which row is to be removed. Otherwise, it will be an invalid move, and the consequences will be the same as in the previously described cases. 
- **Printing the board state** in the same format as when loading it, allowing for potential gameplay between two programs.
- **Printing the game state**:
    - in_progress - the game is not finished
    - white_win
    - black_win
    - dead_lock <color> - the turn of the player currently playing, who cannot make a move
    - bad_move <color> <xN - yM> - the player made an invalid move, additionally printing the player's color and the command that generated the invalid move

