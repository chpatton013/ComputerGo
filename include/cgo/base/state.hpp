#pragma once

#include <array>
#include <tuple>
#include <vector>
#include <boost/optional.hpp>
#include <cgo/base/types.hpp>

namespace cgo {
namespace base {

class State {
public:
   State();
   State(const State& state);
   State(const Board& board);
   ~State();

   const Marker& getMarker(const Position& position) const;

   const Board& getBoard() const;
   std::tuple< int, int > getScores() const;
   int getScore(Marker marker) const;
   std::vector< Position > getLiberties(Marker marker);
   std::vector< Successor > getSuccessors(Marker marker,
    boost::optional< Predecessor > predecessor) const;
   bool isActionValid(const Action& action,
    boost::optional< Predecessor > predecessor) const;

   State& operator=(const State& rhs);

   static struct InvalidMarker {} _invalidMarker;
   static struct InvalidPosition {} _invalidPosition;

   static State applyAction(const State& sourceState, const Action& action);

   static void validateMarker(const Marker& marker);
   static void validatePlayerMarker(const Marker& marker);
   static void validatePosition(const Position& position);

   static void printBoard(const State& state);
   static void printBoard(const Board& board);

   static unsigned int getIndex(const Position& position);
   static Position getPosition(unsigned int index);

private:
   std::vector< Position > calculateLiberties(Marker marker);
   std::vector< std::vector< Position > > groupTerritory() const;

   static std::vector< Position > getAdjacentPositions(const Position& position);
   static std::vector< Position > getSurroundingPositions(const Position& position);
   static void capturePosition(Board& board, Marker marker,
    const Position& position);
   static void collectPositions(const Board& board, Marker marker,
    const Position& position, std::vector< Position >& accumulator,
    std::array< bool, BOARD_DIMENSION * BOARD_DIMENSION >& collected);
   static bool isCaptured(const Board& board,
    const std::vector< Position > positionGroup);
   static void capturePositionGroup(Board& board,
    const std::vector< Position > positionGroup);
   static std::vector< Position > getSurroundingPositionGroup(const Board& board,
    const std::vector< Position > positionGroup, Marker marker);

   Board _board;
   boost::optional< std::vector< Position > > _liberties[2];
};

} // namespace base
} // namespace cgo
