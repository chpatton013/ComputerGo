#pragma once

#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>

namespace cgo {
namespace standardin {

class Node {
public:
   int visits = 0;
   int wins = 0;
   Position position;
   Node child;
   Node sibling;
   Node (Position p);
   update (int val);
   getWinRate ();
};


} // namespace standardin
} // namespace cgo
