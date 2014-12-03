#pragma once

#include <tuple>
#include <boost/variant.hpp>

#ifndef BOARD_DIMENSION
#define BOARD_DIMENSION 9
#endif

namespace cgo {
namespace base {

struct Position {
   Position(int r, int c);
   virtual ~Position();

   bool operator==(const Position& rhs) const;

   int row;
   int col;
};

enum Marker { none, black, white, first = none, last = white };

struct Action {
   Action(Marker m, Position p);
   virtual ~Action();

   Marker marker;
   Position position;
};

struct Pass {};

typedef boost::variant< Pass, Action > Move;

typedef std::array< Marker, BOARD_DIMENSION * BOARD_DIMENSION > Board;

class State;

typedef std::tuple< Move, State > Predecessor;
typedef std::tuple< Move, State > Successor;

} // namespace base
} // namespace cgo
