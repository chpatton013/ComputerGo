#include <cgo/base/state.hpp>

using namespace cgo::base;

State::State() :
   State(Board())
{}

State::State(const State::State& state) :
   State(state._board)
{}

State::State(const State::Board& board) :
   _board(board)
{}

State::~State() {}

const Marker& State::getMarker(const Position& position) const {
   if (position.row < 0 || position.row >= BOARD_DIMENSION ||
    position.col < 0 || position.col >= BOARD_DIMENSION) {
      throw State::_positionOutOfBounds;
   }

   return this->_board[this->getIndex(position)];
}

// TODO: implement me!
std::tuple< int, int > State::getScore() const {
   return {0, 0};
}

std::vector< Position > State::getLiberties(Marker marker) {
   int libertyIndex;
   if (marker == black) {
      libertyIndex = 0;
   } else if (marker == white) {
      libertyIndex = 1;
   } else {
      throw State::_invalidMarker;
   }

   std::vector< Position > liberties;
   if (!this->_liberties[libertyIndex]) {
      liberties = this->calculateLiberties(marker);
      this->_liberties[libertyIndex] = liberties;
   }

   return liberties;
}

std::vector< std::tuple< Move, State > > State::getSuccessors(Marker marker) {
   if (marker < first || marker > last) {
      throw State::_invalidMarker;
   }

   std::vector< std::tuple< Move, State > > successors;

   successors.push_back({Pass(), *this});

   for (Position position : this->getLiberties(marker)) {
      Action action = {marker, position};

      Board successorBoard = this->_board;
      successorBoard[this->getIndex(position)] = marker;
      State successor = State(successorBoard);

      successors.push_back({action, successor});
   }

   return successors;
}

unsigned int State::getIndex(const Position& position) const {
   return position.row * BOARD_DIMENSION + position.col;
}

Position State::getPosition(unsigned int index) const {
   return Position(index / BOARD_DIMENSION, index % BOARD_DIMENSION);
}

std::vector< Position > State::calculateLiberties(Marker marker) {
   std::array< bool, BOARD_DIMENSION * BOARD_DIMENSION > hash;
   hash.fill(false);

   std::vector< Position > liberties;

   for (int row = 0; row < BOARD_DIMENSION; ++row) {
      for (int col = 0; col < BOARD_DIMENSION; ++col) {
         unsigned int index = this->getIndex(Position(row, col));

         if (this->_board[index] != marker) {
            continue;
         }

         std::array< Position, 4 > adjacents = {{
            Position(row - 1, col),
            Position(row, col - 1),
            Position(row + 1, col),
            Position(row, col + 1),
         }};
         for (Position adjPos : adjacents) {
            if (adjPos.row < 0 || adjPos.row >= BOARD_DIMENSION ||
             adjPos.col < 0 || adjPos.col >= BOARD_DIMENSION) {
               continue;
            }

            unsigned int adjIndex = this->getIndex(adjPos);

            if (hash[adjIndex]) {
               continue;
            }

            if (this->_board[adjIndex] != none) {
               continue;
            }

            hash[adjIndex] = true;
            liberties.push_back(adjPos);
         }
      }
   }

   return liberties;
}

/* static */ State::InvalidMarker State::_invalidMarker;
/* static */ State::PositionOutOfBounds State::_positionOutOfBounds;
