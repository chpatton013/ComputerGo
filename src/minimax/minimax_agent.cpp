#include <limits>
#include <cgo/minimax/minimax_agent.hpp>

using namespace cgo::base;
using namespace cgo::minimax;

MiniMaxAgent::MiniMaxAgent(Marker marker) :
   Agent(marker), _turnNumber(0)
{}

/* virtual */ MiniMaxAgent::~MiniMaxAgent() {}

Move MiniMaxAgent::makeMove(State& state,
 const boost::optional< Predecessor >& predecessor) {
   int alpha = std::numeric_limits<int>::min();
   int beta = std::numeric_limits<int>::max();
   int depth;

   if (this->_turnNumber < 5) {
      depth = 2;
   } else if (this->_turnNumber < 10) {
      depth = 3;
   } else if (this->_turnNumber < 15) {
      depth = 4;
   } else if (this->_turnNumber < 20) {
      depth = 5;
   } else {
      depth = 6;
   }

   ++this->_turnNumber;

   auto maxMoveValue = this->mmAbMax(state, predecessor, alpha, beta, depth);
   return std::get<0>(maxMoveValue);
}

std::tuple< Move, int > MiniMaxAgent::mmAbMax(State& state, 
 const boost::optional< Predecessor >& predecessor, int alpha, int beta,
 int depth) {
   Move maxMove = Pass();
   int maxValue = this->utility(state);

   if (predecessor) {
      auto predecessorValue = predecessor.get();
      Move predMove = std::get<0>(predecessorValue);
      Pass* pass = boost::get< Pass >(&predMove);
      if ((pass != nullptr) && (maxValue > 0)) {
         return std::make_tuple(Pass(), maxValue);
      }
   }

   std::vector< Successor > successors =
    state.getSuccessors(this->_marker, predecessor);
   if (depth == 0) {
      if (successors.size() > 0) {
         Move chosenMove = std::get<0>(successors[0]);
         int chosenScore = std::numeric_limits<int>::min();

         for (auto successor : successors) {
            auto succMove = std::get<0>(successor);
            auto succState = std::get<1>(successor);
            int succScore = this->utility(succState);

            if (succScore > chosenScore) {
               chosenMove = succMove;
               chosenScore = succScore;
            }
         }

         return std::make_tuple(chosenMove, chosenScore);
      } else {
         return std::make_tuple(maxMove, maxValue);
      }
   }

   for (auto successor : successors) {
      auto succMove = std::get<0>(successor);
      auto succState = std::get<1>(successor);

      auto minMoveValue = this->mmAbMin(succState, successor, alpha, beta,
       depth - 1);
      auto minMove = std::get<0>(minMoveValue);
      auto minValue = std::get<1>(minMoveValue);
      if (minValue > maxValue) {
         maxMove = minMove;
         maxValue = minValue;
      }

      if (maxValue >= beta) {
         return std::make_tuple(maxMove, maxValue);
      }

      if (alpha < maxValue) {
         alpha = maxValue;
         maxMove = succMove;
      }
   }

   return std::make_tuple(maxMove, maxValue);
}

std::tuple< Move, int > MiniMaxAgent::mmAbMin(State& state, 
 const boost::optional< Predecessor >& predecessor, int alpha, int beta,
 int depth) {
   Move minMove = Pass();
   int minValue = this->utility(state);

   if (predecessor) {
      auto predecessorValue = predecessor.get();
      Move predMove = std::get<0>(predecessorValue);
      Pass* pass = boost::get< Pass >(&predMove);
      if ((pass != nullptr) && (minValue < 0)) {
         return std::make_tuple(Pass(), minValue);
      }
   }

   std::vector< Successor > successors =
    state.getSuccessors(this->_marker, predecessor);
   if (depth == 0) {
      if (successors.size() > 0) {
         Move chosenMove = std::get<0>(successors[0]);
         int chosenScore = std::numeric_limits<int>::max();

         for (auto successor : successors) {
            auto succMove = std::get<0>(successor);
            auto succState = std::get<1>(successor);
            int succScore = this->utility(succState);

            if (succScore < chosenScore) {
               chosenMove = succMove;
               chosenScore = succScore;
            }
         }

         return std::make_tuple(chosenMove, chosenScore);
      } else {
         return std::make_tuple(minMove, minValue);
      }
   }

   for (auto successor : successors) {
      auto succMove = std::get<0>(successor);
      auto succState = std::get<1>(successor);

      auto maxMoveValue = this->mmAbMax(succState, successor, alpha, beta,
       depth - 1);
      auto maxMove = std::get<0>(maxMoveValue);
      auto maxValue = std::get<1>(maxMoveValue);
      if (maxValue < minValue) {
         minMove = maxMove;
         minValue = maxValue;
      }

      if (minValue <= beta) {
         return std::make_tuple(minMove, minValue);
      }

      if (beta > minValue) {
         beta = minValue;
         minMove = succMove;
      }
   }

   return std::make_tuple(minMove, minValue);
}

int MiniMaxAgent::utility(const State& state) {
   auto scores = state.getScores();
   int difference = std::get<0>(scores) - std::get<1>(scores);

   return (this->_marker == white) ? difference : -difference;
}