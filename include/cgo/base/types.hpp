#pragma once

#include <boost/variant.hpp>

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

} // namespace base
} // namespace cgo
