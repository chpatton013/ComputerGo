#pragma once

#include <boost/variant.hpp>

namespace cgo {
namespace base {

struct Position {
   Position(int _x, int _y);
   virtual ~Position();

   int x;
   int y;
};

enum Marker { none, black, white, first = none, last = white };

struct Action {
   Marker marker;
   Position position;
};

struct Pass {};

typedef boost::variant< Pass, Action > Move;

} // namespace base
} // namespace cgo
