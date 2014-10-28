#pragma once

#include <array>
#include <set>
#include <tuple>
#include <vector>
#include <boost/optional.hpp>
#include <cgo/base/types.hpp>

namespace cgo {
namespace base {

#ifndef BOARD_DIMENSION
#define BOARD_DIMENSION 9
#endif

class State {
public:
   typedef std::array< Marker, BOARD_DIMENSION * BOARD_DIMENSION > Board;

   State();
   State(const State& state);
   State(const Board& board);
   ~State();

   const Marker& getMarker(const Position& position) const;

   const Board& getBoard() const;
   std::tuple< int, int > getScores() const;
   int getScore(Marker marker) const;
   std::vector< Position > getLiberties(Marker marker);
   std::vector< std::tuple< Move, State > > getSuccessors(Marker marker,
    boost::optional< std::tuple< Move, State > > predecessor) const;
   bool isActionValid(const Action& action,
    boost::optional< std::tuple< Move, State > > predecessor) const;

   State& operator=(const State& rhs);

   static struct InvalidMarker {} _invalidMarker;
   static struct InvalidPosition {} _invalidPosition;

   static State applyAction(const State& sourceState, const Action& action);

   static void validateMarker(const Marker& marker);
   static void validatePlayerMarker(const Marker& marker);
   static void validatePosition(const Position& position);

   static void printBoard(const State& state);
   static void printBoard(const State::Board& board);

private:
   std::vector< Position > calculateLiberties(Marker marker);

   static unsigned int getIndex(const Position& position);
   static Position getPosition(unsigned int index);
   static std::vector< Position > getAdjacentPositions(const Position& position);
   static std::vector< Position > getSurroundingPositions(const Position& position);

   Board _board;
   boost::optional< std::vector< Position > > _liberties[2];
};

} // namespace base
} // namespace cgo
