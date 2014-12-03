#pragma once

#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>

namespace cgo {
namespace minimax {

class MiniMaxAgent : public base::Agent {
public:
   MiniMaxAgent(base::Marker marker);
   virtual ~MiniMaxAgent();

   base::Move makeMove(base::State& state,
    const boost::optional< base::Predecessor >& predecessor);

private:
   std::tuple< base::Move, int > mmAbMax(base::State& state, 
    const boost::optional< base::Predecessor >& predecessor, int alpha,
    int beta, int depth);
   std::tuple< base::Move, int > mmAbMin(base::State& state, 
    const boost::optional< base::Predecessor >& predecessor, int alpha,
    int beta, int depth);
   std::tuple< int, int> edgeCosts(const base::Board& board);
   int utility(const base::Move& move, const base::State& state);

   int _turnNumber;
};

} // namespace minimax
} // namespace cgo
