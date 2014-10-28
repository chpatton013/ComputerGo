#include <iostream>
#include <cgo/base/state.hpp>

using namespace cgo::base;

State::State() :
   State(Board())
{}

State::State(const State& state) :
   State(state._board)
{}

State::State(const State::Board& board) :
   _board(board)
{}

State::~State() {}

const Marker& State::getMarker(const Position& position) const {
   State::validatePosition(position);

   return this->_board[State::getIndex(position)];
}

std::tuple< int, int > State::getScores() const {
   int whiteScore = 0;
   int blackScore = 0;

   // Count number of pieces.
   for (int ndx = 0; ndx < BOARD_DIMENSION * BOARD_DIMENSION; ++ndx) {
      if (this->_board[ndx] == white) {
         ++whiteScore;
      } else if (this->_board[ndx] == black) {
         ++blackScore;
      }
   }

   // Calculate territory controlled.
   // Unimplemented.

   // Add pieces taken.
   // Unimplemented.
   return std::make_tuple(whiteScore, blackScore);
}

const State::Board& State::getBoard() const { return this->_board; }

int State::getScore(Marker marker) const {
   State::validatePlayerMarker(marker);

   auto score = this->getScores();
   if (marker == white) {
      return std::get<0>(score);
   } else {
      return std::get<1>(score);
   }
}

std::vector< Position > State::getLiberties(Marker marker) {
   State::validatePlayerMarker(marker);

   int libertyIndex = (marker == black) ? 0 : 1;

   std::vector< Position > liberties;

   if (!this->_liberties[libertyIndex]) {
      liberties = this->calculateLiberties(marker);
      this->_liberties[libertyIndex] = liberties;
   }

   return liberties;
}

std::vector< std::tuple< Move, State > > State::getSuccessors(Marker marker,
 boost::optional< std::tuple< Move, State > > predecessor) const {
   State::validatePlayerMarker(marker);

   std::vector< std::tuple< Move, State > > successors;
   for (int row = 0; row < BOARD_DIMENSION; ++row) {
      for (int col = 0; col < BOARD_DIMENSION; ++col) {
         Position position(row, col);
         Action action = {marker, position};

         if (this->isActionValid(action, predecessor)) {
            successors.push_back(std::make_tuple(action, State::applyAction(*this, action)));
         }
      }
   }

   successors.push_back(std::make_tuple(Pass(), *this));

   return successors;
}

bool State::isActionValid(const Action& action,
 boost::optional< std::tuple< Move, State > > predecessor) const {
   Marker marker = action.marker;
   State::validatePlayerMarker(marker);

   Position position = action.position;
   State::validatePosition(position);

   int index = State::getIndex(position);

   if (this->_board[index] != none) {
      return false;
   }

   return true;
}

State& State::operator=(const State::State& rhs) {
   this->_board = rhs._board;
   return *this;
}

/* static */ State State::applyAction(const State& sourceState,
 const Action& action) {
   Marker marker = action.marker;
   Marker opponentMarker = (marker == white) ? black : white;
   State::validatePlayerMarker(marker);

   Position position = action.position;
   State::validatePosition(position);

   int index = State::getIndex(position);

   Board successorBoard = sourceState._board;

   // Take the position.
   successorBoard[index] = marker;
   // Capture enemies.
   State::capturePosition(successorBoard, opponentMarker, position);
   // Capture self.
   State::capturePosition(successorBoard, marker, position);

   return State(successorBoard);
}

/* static */ void State::validateMarker(const Marker& marker) {
   if (marker < first || marker > last) {
      throw State::_invalidMarker;
   }
}

/* static */ void State::validatePlayerMarker(const Marker& marker) {
   State::validateMarker(marker);
   if (marker == none) {
      throw State::_invalidMarker;
   }
}

/* static */ void State::validatePosition(const Position& position) {
   if (position.row < 0 || position.row >= BOARD_DIMENSION ||
    position.col < 0 || position.col >= BOARD_DIMENSION) {
      throw State::_invalidPosition;
   }
}

/* static */ void State::printBoard(const State& state) {
   State::printBoard(state._board);
}

/* static */ void State::printBoard(const State::Board& board) {
   std::cout << "+---+-------------------+" << std::endl;
   std::cout << "|   | 1 2 3 4 5 6 7 8 9 |" << std::endl;
   std::cout << "+---+-------------------+" << std::endl;

   for (int row = 0; row < BOARD_DIMENSION; ++row) {
      std::cout << "| " << (row + 1) << " | ";

      for (int col = 0; col < BOARD_DIMENSION; ++col) {
         int index = row * BOARD_DIMENSION + col;

         switch (board[index]) {
         case none:
            std::cout << "- ";
            break;
         case white:
            std::cout << "W ";
            break;
         case black:
            std::cout << "B ";
            break;
         }
      }

      std::cout << "|" << std::endl;
   }

   std::cout << "+---+-------------------+" << std::endl;
}

std::vector< Position > State::calculateLiberties(Marker marker) {
   std::array< bool, BOARD_DIMENSION * BOARD_DIMENSION > seen;
   seen.fill(false);

   std::vector< Position > liberties;

   for (int row = 0; row < BOARD_DIMENSION; ++row) {
      for (int col = 0; col < BOARD_DIMENSION; ++col) {
         unsigned int index = State::getIndex(Position(row, col));

         if (this->_board[index] != marker) {
            continue;
         }

         for (Position adjacentPosition :
          State::getAdjacentPositions(Position(row, col))) {
            unsigned int adjacentIndex = State::getIndex(adjacentPosition);

            if (seen[adjacentIndex]) {
               continue;
            }

            if (this->_board[adjacentIndex] != none) {
               continue;
            }

            liberties.push_back(adjacentPosition);
            seen[adjacentIndex] = true;
         }
      }
   }

   return liberties;
}

/* static */ unsigned int State::getIndex(const Position& position) {
   return position.row * BOARD_DIMENSION + position.col;
}

/* static */ Position State::getPosition(unsigned int index) {
   return Position(index / BOARD_DIMENSION, index % BOARD_DIMENSION);
}

/* static */ std::vector< Position > State::getAdjacentPositions(
 const Position& position) {
   State::validatePosition(position);

   std::vector< Position > adjacents;

   int row = position.row;
   int col = position.col;

   if (row > 0) {
      adjacents.push_back(Position(row - 1, col));
   }

   if (col > 0) {
      adjacents.push_back(Position(row, col - 1));
   }

   if (row < BOARD_DIMENSION - 1) {
      adjacents.push_back(Position(row + 1, col));
   }

   if (col < BOARD_DIMENSION - 1) {
      adjacents.push_back(Position(row, col + 1));
   }

   return adjacents;
}

/* static */ std::vector< Position > State::getSurroundingPositions(
 const Position& position) {
   State::validatePosition(position);

   std::vector< Position > surrounding = State::getAdjacentPositions(position);

   int row = position.row;
   int col = position.col;

   if (row > 0) {
      if (col > 0) {
         surrounding.push_back(Position(row - 1, col - 1));
      }
      if (col < BOARD_DIMENSION - 1) {
         surrounding.push_back(Position(row - 1, col + 1));
      }
   }
   if (row < BOARD_DIMENSION - 1) {
      if (col > 0) {
         surrounding.push_back(Position(row + 1, col - 1));
      }
      if (col < BOARD_DIMENSION - 1) {
         surrounding.push_back(Position(row + 1, col + 1));
      }
   }

   return surrounding;
}

/* static */ void State::capturePosition(State::Board& board, Marker marker,
 const Position& position) {
   std::array< bool, BOARD_DIMENSION * BOARD_DIMENSION > collected;
   collected.fill(false);

   std::vector< Position > collectedPositions;

   State::collectPositions(board, marker, position, collectedPositions,
    collected);

   auto adjacentPositions = State::getAdjacentPositions(position);
   for (auto adjacentPosition : adjacentPositions) {
      State::collectPositions(board, marker, adjacentPosition,
       collectedPositions, collected);
   }

   bool captured = true;
   for (auto collectedPosition : collectedPositions) {
      auto adjacentCollectedPositions =
       State::getAdjacentPositions(collectedPosition);
      for (auto adjacentPosition : adjacentCollectedPositions) {
         int adjacentIndex = State::getIndex(adjacentPosition);
         if (board[adjacentIndex] == none) {
            captured = false;
            break;
         }
      }
   }

   if (!captured) {
      return;
   }

   for (auto collectedPosition : collectedPositions) {
      int collectedIndex = State::getIndex(collectedPosition);
      board[collectedIndex] = none;
   }
}

/* static */ void State::collectPositions(State::Board& board, Marker marker,
 const Position& position, std::vector< Position >& accumulator,
 std::array< bool, BOARD_DIMENSION * BOARD_DIMENSION >& collected) {
   int index = State::getIndex(position);
   if (collected[index] || (board[index] != marker)) {
      return;
   }

   accumulator.push_back(position);
   collected[index] = true;

   auto adjacentPositions = State::getAdjacentPositions(position);
   for (auto adjacentPosition : adjacentPositions) {
      State::collectPositions(board, marker, adjacentPosition, accumulator,
       collected);
   }
}

/* static */ State::InvalidMarker State::_invalidMarker;
/* static */ State::InvalidPosition State::_invalidPosition;
