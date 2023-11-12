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
