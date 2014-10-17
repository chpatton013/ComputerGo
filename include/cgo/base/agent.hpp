#pragma once

#include <cgo/base/types.hpp>

namespace cgo {
namespace base {

class State;

struct Agent {
   Agent();
   virtual ~Agent();

   virtual Move makeMove(State& state) = 0;
};

} // namespace base
} // namespace cgo
