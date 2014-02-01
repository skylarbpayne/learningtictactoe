learningtictactoe
=================

This is an implementation of a tic tac toe learner.

To compile this project, please remember to use the std=c++11 compiler flag because an unordered map is used.

So your compilation command should like something like:

g++ -std=c++11 ttt.cpp -o appname

The CLI will prompt you to pick a mode for the learner (explore or exploit). Explore seeks to fill out its state table, and converge each state into a winning probability. Exploit always uses what it knows and picks the best choice. So to train it, use the explore method, and exploit to test.

After that, you can pick a random, strategy, or human opponent, and select a number of games to simulate.

Note: The human player is you, this is not networked to some person waiting for you to play the game!

As always, if you have trouble compiling and/or using, please contact me.
