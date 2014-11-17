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

   int makeRandomMove();
   int playRandomGame();
   int createChildren(Node& node);
   Node getBestChild(Node &root);
   Node UCTSelect(Node& node);
   int playSimulation(Node& node);
   base::Move makeMove(base::State& state,
    const boost::optional< std::tuple< base::Move, base::State > >& predecessor);

private:
   int CalculateBest(base::Position position);
};


} // namespace standardin
} // namespace cgo
