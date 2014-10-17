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
   if (position.x < 0 || position.x >= BOARD_DIMENSION ||
    position.y < 0 || position.y >= BOARD_DIMENSION) {
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

   if (!this->_liberties[libertyIndex]) {
      this->_liberties[libertyIndex] = this->calculateLiberties(marker);
   }

   // Just returning this->_liberties[libertyIndex].get() without explicitly
   // checking the thruthyness of the optional will silently fail by ALWAYS
   // returning a default (empty) vector.
   auto libertyOpt = this->_liberties[libertyIndex];
   if (!libertyOpt) {
      return std::vector< Position >();
   } else {
      return libertyOpt.get();
   }
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
   return position.y * BOARD_DIMENSION + position.x;
}

Position State::getPosition(unsigned int index) const {
   return Position(index / BOARD_DIMENSION, index % BOARD_DIMENSION);
}

std::vector< Position > State::calculateLiberties(Marker marker) {
   std::vector< Position > liberties;

   bool hash[BOARD_DIMENSION * BOARD_DIMENSION] = {0};
   for (int x = 0; x < BOARD_DIMENSION; ++x) {
      for (int y = 0; y < BOARD_DIMENSION; ++y) {
         unsigned int index = this->getIndex(Position(x, y));

         if (this->_board[index] != marker) {
            continue;
         }

         std::array< Position, 4 > adjacents = {{
            Position(x - 1, y),
            Position(x + 1, y),
            Position(x, y - 1),
            Position(x, y + 1),
         }};
         for (Position adjPos : adjacents) {
            if (adjPos.x < 0 || adjPos.x >= BOARD_DIMENSION ||
             adjPos.y < 0 || adjPos.y >= BOARD_DIMENSION) {
               continue;
            }

            unsigned int adjIndex = this->getIndex(adjPos);

            if (!hash[adjIndex]) {
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
