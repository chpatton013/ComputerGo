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
    const boost::optional< base::Predecessor >& predecessor);
};

} // namespace standardin
} // namespace cgo
