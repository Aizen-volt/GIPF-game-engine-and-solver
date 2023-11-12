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

## Solver
The solver should allow:
- **Generating all possible moves** for the active player
- **Evaluating the game** by answering the question of whether the game has ended and if the player has won/drawn/lost. Note that this is equivalent to answering the question of whether the active player can make any move. If not, they have lost, and their opponent has won.
- **Solving the game** on a small board or end states on a more complex board.
- Answering the question of **whether the active player can win** within a given number of moves for a given game state (a move is understood as a move of a single pawn, not both).

## Commands
- **LOAD_GAME_BOARD** followed by the game parameters and the state of the board. For example, loading the original board would look like this:

```
LOAD_GAME_BOARD
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

It should be checked whether the provided board state is valid, whether the sum of pawns for both players on the board and in reserves is correct, i.e., identical to the one provided in list L. It should also be checked whether any sequences of pawns triggering their collection have been removed. 
- **PRINT_GAME_BOARD** prints the current state of the board. After loading the game, it should be exactly the same as the printout provided during loading, assuming it was valid.
- **DO_MOVE <xN - yM>** performs the given move for the active player on the current board. The move notation should be consistent with the notation used in the **Gipf for One (GF1)** program. If the move is valid, a new board will be generated, and the active player will switch to the opponent. The game may end if the opponent no longer has any reserve pieces. If the move is invalid, the appropriate game state should be established as "bad_move," and the player and move that caused this state should be remembered. In cases where a player needs to remove pieces from the board and there are multiple possibilities, he must decide which pieces he want to remove. This information must be added to the command in the form of coordinates for those pieces. The coordinates of the pieces are provided according to the scheme described in the example of the initial board for the standard version of the game:

```
    + + + + + 
   + W _ _ B +
  + _ _ _ _ _ +
 + _ _ _ _ _ _ +
+ B _ _ _ _ _ W +
 + _ _ _ _ _ _ +
  + _ _ _ _ _ +
   + W _ _ B +
    + + + + +
```

Indexes of the fields for the above board:

```
    a5 b6 c7 d8 e9
   a4 b5 c6 d7 e8 f8
  a3 b4 c5 d6 e7 f7 g7
 a2 b3 c4 d5 e6 f6 g6 h6
a1 b2 c3 d4 e5 f5 g5 h5 i5
  b1 c2 d3 e4 f4 g4 h4 i4
    c1 d2 e3 f3 g3 h3 i3
      d1 e2 f2 g2 h2 i2
        e1 f1 g1 h1 i1
```

Indexes of the fields for the smaller boars:

```

  a3 b4 c5
 a2 b3 c4 d4
a1 b2 c3 d3 e3
  b1 c2 d2 e2
    c1 d1 e1

   a4 b5 c6 d7
  a3 b4 c5 d6 e6
 a2 b3 c4 d5 e5 f5
a1 b2 c3 d4 e4 f4 g4
  b1 c2 d3 e3 f3 g3
    c1 d2 e2 f2 g2
      d1 e1 f1 g1

```

Therefore, in the case of a move that leads to an ambiguous situation requiring clarification of which pieces should be removed, the command includes the color of the player for whom the choice applies, followed by a sequence of coordinates specifying the removed pieces. The command will look as follows:
```
DO_MOVE <xN - yM> [w:|b:]x1 xn ... 
```
If there are multiple choices, they will be provided as subsequent sequences of indexes. 
- **PRINT_GAME_STATE** prints the current game state ("in_progress," "bad_move," etc.).
- **GEN_ALL_POS_MOV** prints list of all possible moves (without repetitions) leading to unique board states.
- **GEN_ALL_POS_MOV_EXT** works the same as GEN_ALL_POS_MOV, except when there is any winning move available, it only prints one of those moves.
- **GEN_ALL_POS_MOV_NUM** prints the number of all moves obtained as a result of the GEN_ALL_POS_MOV command.
- **GEN_ALL_POS_MOV_EXT_NUM** works the same as GEN_ALL_POS_MOV_NUM, except when there is any winning move available, it prints 1.
- **WINNING_SEQUENCE_EXIST <N>** answers whether there exists a winning sequence of moves, of length less than or equal to N, for any of the players.
- **SOLVE_GAME_STATE** answers the question of which player has a winning strategy for the given game state. The possible answers are WHITE_HAS_WINNING_STRATEGY or BLACK_HAS_WINNING_STRATEGY.

## Example
### Input:

```
LOAD_GAME_BOARD
4 4 15 15
6 3 W
   _ _ _ W
  _ _ _ W _
 _ _ _ W _ _
W W W _ _ _ _
 _ _ _ W _ _
  _ _ _ W _
   _ _ _ W

GEN_ALL_POS_MOV_NUM
GEN_ALL_POS_MOV
```

### Output:

```
BOARD_STATE_OK
18_UNIQUE_MOVES
4 4 15 15
5 3 B
   W _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ W _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ W W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W W   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
9 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ _ _ _ 
  _ _ _ _ _ 
   _ _ _ _ 
4 4 15 15
9 3 B
   _ _ _ _    
  _ _ _ _ _   
 _ _ _ _ _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
9 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
_ _ _ _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ W  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ W 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ W 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W W 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ W W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ W _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   W _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  W _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 W _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  _ _ _ W _   
 W _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
4 4 15 15
5 3 B
   _ _ _ W    
  W _ _ W _   
 _ _ _ W _ _  
W W W _ _ _ _ 
 _ _ _ W _ _ 
  _ _ _ W _ 
   _ _ _ W 
```
