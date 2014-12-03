#include <limits>
#include <cmath>
#include <cgo/minimax/minimax_agent.hpp>

using namespace cgo::base;
using namespace cgo::minimax;

MiniMaxAgent::MiniMaxAgent(Marker marker) :
   Agent(marker), _turnNumber(0)
{}

/* virtual */ MiniMaxAgent::~MiniMaxAgent() {}

Move MiniMaxAgent::makeMove(State& state,
 const boost::optional< Predecessor >& predecessor) {
   // Make an intelligent first move.
   if (this->_turnNumber == 0) {
      ++this->_turnNumber;
      return this->makeFirstMove(state);
   }

   ++this->_turnNumber;
   return this->makeMiniMaxMove(state, predecessor);
}

Action MiniMaxAgent::makeFirstMove(const State& state) const {
   const int crucialPoint = std::ceil(BOARD_DIMENSION / 3.0f) - 1;

   Position p1(crucialPoint, crucialPoint);
   if (state.getBoard()[State::getIndex(p1)] == none) {
      return Action(this->_marker, p1);
   }

   Position p2(crucialPoint, (BOARD_DIMENSION - 1) - crucialPoint);
   return Action(this->_marker, p2);
}

Move MiniMaxAgent::makeMiniMaxMove(State& state,
 const boost::optional< Predecessor >& predecessor) const {
   int alpha = std::numeric_limits<int>::min();
   int beta = std::numeric_limits<int>::max();
   int depth = this->getDepth(state);
   auto maxMoveValue = this->mmAbMax(state, predecessor, alpha, beta, depth);
   return std::get<0>(maxMoveValue);
}

std::tuple< Move, int > MiniMaxAgent::mmAbMax(State& state, 
 const boost::optional< Predecessor >& predecessor, int alpha, int beta,
 int depth) const {
   Move maxMove = Pass();
   int maxValue = this->utility(maxMove, state);

   if (predecessor) {
      auto predecessorValue = predecessor.get();
      Move predMove = std::get<0>(predecessorValue);
      const Pass* pass = boost::get< Pass >(&predMove);
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
            int succScore = this->utility(succMove, succState);

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
 int depth) const {
   Move minMove = Pass();
   int minValue = this->utility(minMove, state);

   if (predecessor) {
      auto predecessorValue = predecessor.get();
      Move predMove = std::get<0>(predecessorValue);
      const Pass* pass = boost::get< Pass >(&predMove);
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
            int succScore = this->utility(succMove, succState);

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

std::tuple< int, int > MiniMaxAgent::edgeCosts(const Board& board) const {
   int whiteCost = 0;
   int blackCost = 0;

   for (int ndx = 0; ndx < BOARD_DIMENSION; ++ndx) {
      Marker rowMarker1 = board[State::getIndex(Position(ndx, 0))];
      Marker rowMarker2 = board[State::getIndex(Position(ndx, BOARD_DIMENSION - 1))];
      Marker colMarker1 = board[State::getIndex(Position(0, ndx))];
      Marker colMarker2 = board[State::getIndex(Position(BOARD_DIMENSION - 1, ndx))];

      if (rowMarker1 == white) {
         ++whiteCost;
      }
      if (rowMarker2 == white) {
         ++whiteCost;
      }
      if (colMarker1 == white) {
         ++whiteCost;
      }
      if (colMarker2 == white) {
         ++whiteCost;
      }

      if (rowMarker1 == black) {
         ++blackCost;
      }
      if (rowMarker2 == black) {
         ++blackCost;
      }
      if (colMarker1 == black) {
         ++blackCost;
      }
      if (colMarker2 == black) {
         ++blackCost;
      }
   }

   return std::make_tuple(whiteCost, blackCost);
}

int MiniMaxAgent::utility(const Move& move, const State& state) const {
   static const int scoreWeight = 1000;
   static const int edgeWeight = 10;
   static const int passWeight = 1;

   auto scores = state.getScores();
   int scoreValue = std::get<0>(scores) - std::get<1>(scores);

   auto edgeCosts = MiniMaxAgent::edgeCosts(state.getBoard());
   int edgeValue = (this->_marker == white) ? -std::get<0>(edgeCosts) :
    std::get<1>(edgeCosts);

   int passValue = 0;
   const Pass* pass = boost::get< Pass >(&move);
   if (pass != nullptr) {
      passValue = (this->_marker == white) ? 1 : -1;
   }

   int totalValue = scoreWeight * scoreValue + edgeWeight * edgeValue +
    passWeight * passValue;

   return (this->_marker == white) ? totalValue : -totalValue;
}

int MiniMaxAgent::getDepth(const base::State& state) const {
   static const int maxNumCases = 10E4;

   // Count open positions.
   int numOpenPositions = 0;
   for (int row = 0; row < BOARD_DIMENSION; ++row) {
      for (int col = 0; col < BOARD_DIMENSION; ++col) {
         int index = State::getIndex(Position(row, col));
         if (state.getBoard()[index] == none) {
            ++numOpenPositions;
         }
      }
   }
   // Factorial.
   int numPossibleCases = 1;
   for (int ndx = numOpenPositions; ndx > 0; --ndx) {
      numPossibleCases *= ndx;

      if (numPossibleCases > maxNumCases) {
         return (numOpenPositions - ndx) + 1;
      }
   }

   return numOpenPositions + 1;
}
