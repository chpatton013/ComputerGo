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


Node getBestChild(Node root) {
   Node child = root.child;
   Node best_child = null;
   int best_visits= -1;
   while (child!=0) { // for all children
      if (child.visits>best_visits) {
          best_child=child;
          best_visits=child.visits;
      }
      child = child.sibling;
   }
   return best_child;
}

//state -> construct new state and pass the board , use applyAction, state 

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

int MonteCarloAgent::playSimulation(Node n) {
   int randomresult = 0;
   if (n.child == null && n.visits < 10) {
      randomresult = playRandomGame(); // Change to whatever play random is
   }
   else {
      if (n.child == null) {
         createChildren(n);
      }

      Node next = UCTSelect(n);
      if (next == 0) {
         
      }

      int res = playSimulation(next);
      randomresult = 1 - res;
   }

   n.update(1-randomresult);
   return randomresult;
}

static const numSims = 300000; 
Move MonteCarloAgent::makeMove(State& state,
 const boost::optional< std::tuple< Move, State > >& predecessor) {
   int i = 0;
   Position position(-1, -1);
   root = new Node(position);
   State clone = State(state);
   Board boardClone; = clone.getBoard();
   for (i = 0; i < numSims; i++) {
      boardClone = clone.getBoard();
      playSimulation(root);
   }

   Node n = getBestChild(root);
   return Action(this->_marker, n.position);
}

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



  // return Action(this->_marker, position);
//}
