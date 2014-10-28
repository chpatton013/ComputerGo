# ComputerGo

## Prerequesites
* You must have `g++` (or `clang`), `git`, and `premake4` installed.
* `$LD_LIBRARY_PATH` must contain the location of the built libraries.
  * Add this to your `~/.bashrc`: `LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib`

## Building
```
git submodule update --init --recursive
premake4 gmake
make
```

## Modules

### cgo_base
The heart and soul of the application. This modules contains the State object
and Agent interface, as well as the game logic.

### cgo_stdin
A pseudo agent that uses input from stdin to make moves.

### cgo_random
A pseudo agent that picks random board locations to make moves.

### cgo_minimax
An alpha-beta pruned minimax agent. This module provides implementations of the
Agent interface needed to perform AB-MiniMax with a maxmimum depth.

### cgo_montecarlo
A Monte-Carlo randomization agent. This module provides implementations of the
Agent interface needed to perform Monte-Carlo simulations.

### cgo_driver
The executable application. This module parses commandline arguments and
initializes a game between two agents.

### cgo_test
The testing application. Automated tests for all modules are included in this
executable.
