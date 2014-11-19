#pragma once

#include <array>
#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>
#include <cgo/base/types.hpp>

namespace cgo {
namespace driver {

class Driver {
public:
   Driver();
   virtual ~Driver();

   void play();

protected:
   base::Agent* choiceToAgent(int choice, base::Marker marker) const;
   virtual std::array< base::Agent*, 2 > getPlayerAgents() const = 0;
   virtual void announceTurnStart() const = 0;
   virtual void announceTurnEnd() const = 0;
   virtual void announceGameEnd() const = 0;

   base::State _state;
   int _turn;
};

} // namespace driver
} // namespace cgo
