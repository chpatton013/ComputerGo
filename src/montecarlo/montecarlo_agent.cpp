#include <iostream>
#include <string>
#include <sstream>
#include <cgo/montecarlo/montecarlo_agent.hpp>
#include <cgo/montecarlo/montecarlo_tree.hpp>

using namespace cgo::base;
using namespace cgo::standardin;

MonteCarloAgent::MonteCarloAgent(Marker marker) :
   Agent(marker)
{}

/* virtual */ MonteCarloAgent::~MonteCarloAgent() {}

int MonteCarloAgent::CalculateBest(Position position) {
   return 0;
}
static const UCTK = sqrt(1/5);
Node MonteCarloAgent::UCTSelect(Node& node) {
   Node result = null;
   Node next = node.child;

   double best_uct = 0;
   while (next != null) {
      if (next.visits > 0) {
         double winrate = next.getWinRate();
         double uct = UCTK * sqrt(log(node.visits)/ next.visits);
      }
      else {
         uctvalue = 10000 + 1000 * random();
      }

      if (uctvalue > best_uct) {
         best_uct = uctvalue;
         res = next;
      }

      next = next.sibling;
   }
   return res;
}

Move MonteCarloAgent::makeMove(State& state,
 const boost::optional< std::tuple< Move, State > >& predecessor) {
   Position position(-1, -1);

   // do {
   //    std::cout << "Enter a move: <row> <col> or (p)ass: ";

   //    std::string move;
   //    do {
   //       std::getline(std::cin, move);
   //    } while (move.size() == 0);

   //    if (move == "p" || move == "pass") {
   //       return Pass();
   //    }

   //    int row, col;
   //    std::stringstream ss(move);
   //    ss >> row >> col;

   //    position = Position(row - 1, col - 1);
   // } while (!state.isActionValid(Action(this->_marker, position), predecessor));



   return Action(this->_marker, position);
}
