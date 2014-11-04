#include <iostream>
#include <string>
#include <sstream>
#include <cgo/stdin/stdin_agent.hpp>

using namespace cgo::base;
using namespace cgo::standardin;

StdinAgent::StdinAgent(Marker marker) :
   Agent(marker)
{}

/* virtual */ StdinAgent::~StdinAgent() {}

Move StdinAgent::makeMove(State& state,
 const boost::optional< Predecessor >& predecessor) {
   Position position(-1, -1);

   do {
      std::cout << "Enter a move: <row> <col> or (p)ass: ";

      std::string move;
      do {
         std::getline(std::cin, move);
      } while (move.size() == 0);

      if (move == "p" || move == "pass") {
         return Pass();
      }

      int row, col;
      std::stringstream ss(move);
      ss >> row >> col;

      position = Position(row - 1, col - 1);
   } while (!state.isActionValid(Action(this->_marker, position), predecessor));

   return Action(this->_marker, position);
}
