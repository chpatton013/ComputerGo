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
   base::Action makeFirstMove(const base::State& state) const;
   base::Move makeMiniMaxMove(base::State& state,
    const boost::optional< base::Predecessor >& predecessor) const;

   std::tuple< base::Move, int > mmAbMax(base::State& state, 
    const boost::optional< base::Predecessor >& predecessor, int alpha,
    int beta, int depth) const;
   std::tuple< base::Move, int > mmAbMin(base::State& state, 
    const boost::optional< base::Predecessor >& predecessor, int alpha,
    int beta, int depth) const;

   std::tuple< int, int> edgeCosts(const base::Board& board) const;
   int utility(const base::Move& move, const base::State& state) const;

   int getDepth(const base::State& state) const;

   int _turnNumber;
};

} // namespace minimax
} // namespace cgo
