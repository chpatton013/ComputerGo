#pragma once

#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>
#include <cgo/montecarlo/montecarlo_tree.hpp>

namespace cgo {
namespace montecarlo {


class MonteCarloAgent : public base::Agent {
const int BOARD_SIZE = 9;
public:
   MonteCarloAgent(base::Marker marker);
   virtual ~MonteCarloAgent();
   bool checkGameOver(base::State& state, base::Move move,
    const boost::optional< std::tuple< base::Move, base::State > >& predecessor);
   base::Move makeRandomMove(base::State& state,
    const boost::optional< std::tuple< base::Move, base::State > >& predecessor);
   int playRandomGame(base::State& state,
    const boost::optional< std::tuple< base::Move, base::State > >& predecessor);
   int createChildren(Node* node, base::State& state,
    const boost::optional< std::tuple< base::Move, base::State > >& predecessor);
   Node* getBestChild(Node* root);
   Node* UCTSelect(Node* node);
   int playSimulation(Node* node, base::State& state,
    const boost::optional< std::tuple< base::Move, base::State > >& predecessor);
   base::Move makeMove(base::State& state,
    const boost::optional< std::tuple< base::Move, base::State > >& predecessor);
};


} // namespace montecarlo
} // namespace cgo
