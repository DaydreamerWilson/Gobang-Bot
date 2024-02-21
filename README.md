# Gomoku-Bot
A Gobang program with a built-in AI. 

Currently there's a functioning Gobang game and two versions of painfully slow mini-max algorithm with a depth of 4.

Algorithm list:
  Minimax:
    With two functions maximizer() and minimizer(),
    calling the maximizer or the minimizer will call the other one recursively until designated depth reached.
    Alpha-beta pruning is included.
  Simplified Minimax:
    A slightly faster and simplified version of the previous minimax algorithm, it is around 2 time faster but can yield sub-optimal results.

More algorithms will be implemented in the future.
