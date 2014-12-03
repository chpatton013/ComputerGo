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


bool MiniMaxAgent::checkBounds(int x, int y) const{
   if (x < 0) {
      return false;
   }
   if (x >= BOARD_DIMENSION) {
      return false;
   }
   if (y < 0) {
      return false;
   }
   if (y >= BOARD_DIMENSION) {
      return false;
   }
   return true;
}

int MiniMaxAgent::pseudoControl(const Board& board) const{
   int totalControl = 0;
   static const int pieceVal = 5; //value for having a piece on a square
   static const int controlVal = 20; //bonus value for having 'control' over a square
   static const int weakVal = 10; //value for having weak presence on a square
   static const int strongVal = 30; //value for having strong presence on a square
   static const int vStrongVal = 40; //value for having a very strong presence on a square
   std::array<int, BOARD_DIMENSION * BOARD_DIMENSION> wcontrol;
   std::array<int, BOARD_DIMENSION * BOARD_DIMENSION> bcontrol;
   wcontrol.fill(0);
   bcontrol.fill(0);
   for (int x = 0; x < BOARD_DIMENSION; ++x) {
      for (int y = 0; y < BOARD_DIMENSION; ++y) {
         Marker curr = board[State::getIndex(Position(x, y))];
         if (curr == white) {
            if (checkBounds(x, y)) {
               wcontrol[State::getIndex(Position(x, y))]++;
            }
            if (checkBounds(x, y + 1)) {
               wcontrol[State::getIndex(Position(x, y + 1))]++;
            }
            if (checkBounds(x, y - 1)) {
               wcontrol[State::getIndex(Position(x, y - 1))]++;
            }
            if (checkBounds(x + 1, y)) {
               wcontrol[State::getIndex(Position(x + 1, y))]++;
            }
            if (checkBounds(x - 1, y)) {
               wcontrol[State::getIndex(Position(x - 1, y))]++;
            }
         }
         
         if (curr == black) {
            if (checkBounds(x, y)) {
               bcontrol[State::getIndex(Position(x, y))]++;
            }
            if (checkBounds(x, y + 1)) {
               bcontrol[State::getIndex(Position(x, y + 1))]++;
            }
            if (checkBounds(x, y - 1)) {
               bcontrol[State::getIndex(Position(x, y - 1))]++;
            }
            if (checkBounds(x + 1, y)) {
               bcontrol[State::getIndex(Position(x + 1, y))]++;
            }
            if (checkBounds(x - 1, y)) {
               bcontrol[State::getIndex(Position(x - 1, y))]++;
            }
         }
      }
   }
   
   for (int x = 0; x < BOARD_DIMENSION; ++x) {
      for (int y = 0; y < BOARD_DIMENSION; ++y) {
         Marker curr = board[State::getIndex(Position(x, y))];
         int control = 0;
         if (wcontrol[State::getIndex(Position(x, y))] == 1 || wcontrol[State::getIndex(Position(x, y))] == 5) {
            control += weakVal;
         }
         if (wcontrol[State::getIndex(Position(x, y))] == 2 || wcontrol[State::getIndex(Position(x, y))] == 4) {
            control += strongVal;
         }
         if (wcontrol[State::getIndex(Position(x, y))] == 3) {
            control += vStrongVal;
         }
         
         if (bcontrol[State::getIndex(Position(x, y))] == 1 || bcontrol[State::getIndex(Position(x, y))] == 5) {
            control -= weakVal;
         }
         if (bcontrol[State::getIndex(Position(x, y))] == 2 || bcontrol[State::getIndex(Position(x, y))] == 4) {
            control -= strongVal;
         }
         if (bcontrol[State::getIndex(Position(x, y))] == 3) {
            control -= vStrongVal;
         }
         if (curr == white ){
            control += pieceVal;
         }
         if (curr == black){
            control -= pieceVal;
         }
         if (control > 0) {
            control += controlVal;
         }
         if (control < 0) {
            control -= controlVal;
         }
         totalControl += control;
      }
   }
   return totalControl;
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

   auto control = MiniMaxAgent::pseudoControl(state.getBoard());
   control = (this->_marker == white) ? control : -control;
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
    passWeight * passValue + control;

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
