#pragma once

#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>

namespace cgo {
namespace standardin {

class MonteCarloAgent : public base::Agent {
public:
   class Node  {
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
   MonteCarloAgent(base::Marker marker);
   virtual ~MonteCarloAgent();

   base::Move makeMove(base::State& state,
    const boost::optional< std::tuple< base::Move, base::State > >& predecessor);

private:
   CalculateBest(Position position);
};


} // namespace standardin
} // namespace cgo
