# Gomoku-Bot
A Gobang program with a built-in AI. 

Currently there's a functioning Gobang game and two versions of painfully slow mini-max algorithm with a depth of 4.

I started this project wanting to brute-force gomoku, but in the middle of the coding I realized the pathetic reality of computer development nowaday, as I can't find a large enough hard drive I can afford to store the entirety of gomoku itself, or quite literally anywhere else. As a result I wrote a minimax and decided to continue to expand on this project as a playground to test some decision making algorithms. The current minimax and state evaluating function is problematic, to be continued.

# Algorithm list:

## Minimax:
With two functions maximizer() and minimizer(),

calling the maximizer or the minimizer will call the other one recursively until designated depth reached.

Alpha-beta pruning is included.

# Notes
More algorithms will be implemented in the future.
