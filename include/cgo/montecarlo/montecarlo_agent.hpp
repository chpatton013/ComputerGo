#pragma once

#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>
#include <cgo/montecarlo/montecarlo_tree.hpp>
namespace cgo {
namespace standardin {


class MonteCarloAgent : public base::Agent {
const int BOARD_SIZE = 9;	
public:

   MonteCarloAgent(base::Marker marker);
   virtual ~MonteCarloAgent();

   base::void makeRandomMove();
   base::int playRandomGame();
   base::void createChildren(Node& node);
   base::Node getBestChild(Node &root);
   base::Node UCTSelect(Node& node);
   base::int playSimulation(Node& node); 
   base::Move makeMove(base::State& state,
    const boost::optional< std::tuple< base::Move, base::State > >& predecessor);

private:
   CalculateBest(Position position);
};


} // namespace standardin
} // namespace cgo
