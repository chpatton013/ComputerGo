#include <iostream>
#include <sstream>
#include <cgo/stdin/stdin_agent.hpp>

using namespace cgo::base;
using namespace cgo::standardin;

StdinAgent::StdinAgent(Marker marker) :
   Agent(marker)
{}

/* virtual */ StdinAgent::~StdinAgent() {}

Move StdinAgent::makeMove(State& state,
 const std::tuple< Move, State >& predecessor) {
   static const int bufferSize = 256;
   char input[bufferSize];

   Position position(-1, -1);

   do {
      std::cout << "Enter a move: <row> <col> or (p)ass: ";
      std::cin.getline(input, bufferSize);

      std::string move(input);

      if (move == "p" || move == "pass") {
         return Pass();
      }

      int row, col;
      std::stringstream ss(move);
      ss >> row >> col;

      position = Position(row - 1, col -1);
   } while (state.isActionValid(Action(this->_marker, position), predecessor));

   return Action(this->_marker, position);
}
