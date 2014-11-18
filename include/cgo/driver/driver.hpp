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
   void announceGameOver(int i);
   int checkGameOver();
protected:
   base::Agent* choiceToAgent(int choice, base::Marker marker) const;
   virtual std::array< base::Agent*, 2 > getPlayerAgents() const = 0;
   virtual void announceTurn() const = 0;
   virtual void announceTurnEnd() const = 0;

   virtual void announceWinner() const = 0;

   base::State _state;
   int _turn;
   int _gameOver; //1 or 0, reflects game is over when value is 1
};

} // namespace driver
} // namespace cgo
