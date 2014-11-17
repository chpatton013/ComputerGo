#pragma once

#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>

namespace cgo {
namespace standardin {

class Node {
public:
   int visits = 0;
   int wins = 0;
   base::Position position;
   cgo::montecarlo::Node child;
   cgo::montecarlo::Node sibling;
   Node (base::Position p);
   int update (int val);
   double getWinRate ();
};


} // namespace standardin
} // namespace cgo
