#include <boost/optional.hpp>
#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>
#include <cgo/driver/driver.hpp>
#include <cgo/random/random_agent.hpp>
#include <cgo/stdin/stdin_agent.hpp>

using namespace cgo;
using namespace cgo::driver;

Driver::Driver() :
   _state(), _turn(0)
{}

/* virtual */ Driver::~Driver() {
}

void Driver::play() {
   std::array< base::Agent*, 2 > agents = this->getPlayerAgents();
   std::array< boost::optional< base::Predecessor >, 2 > predecessors;
   std::array< bool, 2 > passes;
   passes.fill(false);
   this->announceGameOver(0);

   do {
      int playerIndex = this->_turn % 2;
      int opponentIndex = (this->_turn + 1) % 2;

      this->announceTurn();

      auto predecessor = predecessors[opponentIndex];
      auto move = agents[playerIndex]->makeMove(this->_state, predecessor);

      base::Action* action = boost::get< base::Action >(&move);
      if (action != nullptr) {
         this->_state = base::State::applyAction(this->_state, *action);
         passes[playerIndex] = false;
      } else {
         passes[playerIndex] = true;
      }

      predecessors[playerIndex] = std::make_tuple(move, this->_state);

      ++this->_turn;

      this->announceTurnEnd();
   } while (!passes[0] || !passes[1]);

   this->announceGameOver(1);
   this->announceWinner();
}

void Driver::announceGameOver(int i) {
   this->_gameOver = i;
}

int Driver::checkGameOver() {
   return this->_gameOver;
}


base::Agent* Driver::choiceToAgent(int choice, base::Marker marker) const {
   switch (choice) {
   case 0:
      return new standardin::StdinAgent(marker);
   case 1:
      return new random::RandomAgent(marker);
   }

   return nullptr;
}
