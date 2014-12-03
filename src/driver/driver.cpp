#include <boost/optional.hpp>
#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>
#include <cgo/driver/driver.hpp>
#include <cgo/random/random_agent.hpp>
#include <cgo/stdin/stdin_agent.hpp>
#include <cgo/montecarlo/montecarlo_agent.hpp>
#include <cgo/minimax/minimax_agent.hpp>

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

   do {
      int playerIndex = (this->_turn % 2);
      int opponentIndex = (this->_turn + 1) % 2;

      this->announceTurnStart();

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

      this->announceMove(move);

      ++this->_turn;

      this->announceTurnEnd();
   } while (!passes[0] || !passes[1]);

   this->announceGameEnd();
}

base::Agent* Driver::choiceToAgent(int choice, base::Marker marker) const {
   switch (choice) {
   case 0:
      return new standardin::StdinAgent(marker);
   case 1:
      return new random::RandomAgent(marker);
   case 2:
      return new montecarlo::MonteCarloAgent(marker);
   case 3:
      return new minimax::MiniMaxAgent(marker);
   }

   return nullptr;
}
