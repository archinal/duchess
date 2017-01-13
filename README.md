# Duchess #

This is a C++ implementation of the game [Duchess](https://www.cse.unsw.edu.au:443/~blair/duchess/).

This is the result of an Artificial Intelligence/Machine Learning thesis completed by William Archinal under the supervision of Dr Alan Blair at UNSW in 2016 as part of the requirements of a Bachelor of Engineering (Software Engineering). The paper produced as a result of this work is available in the file [paper.pdf](https://github.com/archinal/duchess/blob/master/paper.pdf) in this repository.

This project includes:

- A library to represent and manage the game (`duchess/`)

- A library to represent a player of the game, supporting both human and AI players (`player/`). This library also includes machine learning code.

- A library to facilitate playing a game (`game/`) including logging

- A library to host a C++ AI and respond to board states sent by the Java Client over TCP (`server/`)

- Various game runners to tie these things together (`main/`)

- Test suites in each of these libraries (`DIR/test`)

## General pointers to the Code ##
- The alpha-beta search is applied in `player/Player.cpp` under `Player::searchForMove`

- All machine learning and board evaluation is performed via `player/Brain.cpp`

- The weights of various stages of the network are under `weights/`

## Running things ##

Generally speaking, all scripts used to build artifacts are located in `buildScripts/` and they tend to compile programs
whose main files reside in `main/`, resulting in executables in the `bin/` folder.

#### Pre-requisites ####

- Valgrind is required to run the tests, as the tests check for memory leaks (install with `$ sudo apt-get install valgrind` or `$ brew install valgrind`)

- C++11 or higher

#### To run the tests ####
Simply run:
```
$ cd buildScripts
$ bash testDuchess.sh
$ bash testPlayer.sh
$ bash testGame.sh
$ bash testServer.sh
```

The tests are unit tests for all of the modular components and helper functions in the project. Furthermore, `testPlayer.sh` includes integration tests making the AI respond to certain board states.
The tests run `valgrind` to ensure that there are no memory leaks in the running of the application.

#### To run the game Server ####
To run a local networked version of the game (i.e. where your computer acts as the "server") run:
```
# Build the game
$ cd buildScripts
$ bash runAIServer.sh

# Launch the Java
$ cd path/to/java/DuchessPlay
$ javac *.java
$ java Duchess
```
Finally in the Java client, "Turn on AI" to play against the C++ AI.

#### To train the AI ####
The relevant configuration for training the AI can be found in `main/runTraining.cpp`. Here you can choose which stages of the network must be trained, how long it should be trained for, and you can specify input and output weights.

Once configuration has been done, simlpy run
```
$ cd buildScripts
$ bash buildRunTraining.sh
$ ../bin/runTraining.o
```

#### Other development pre-requisites ####

- Python 3 to run the scripts in the `scripts/` folder
