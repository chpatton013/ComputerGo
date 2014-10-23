#pragma once

#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>

namespace cgo {
namespace standardin {

class StdinAgent : public base::Agent {
public:
   StdinAgent(base::Marker marker);
   virtual ~StdinAgent();

   base::Move makeMove(base::State& state,
    const std::tuple< base::Move, base::State >& predecessor);
};

} // namespace standardin
} // namespace cgo
