#include <boost/optional.hpp>
#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>
#include <cgo/driver/driver.hpp>
#include <cgo/random/random_agent.hpp>
#include <cgo/stdin/stdin_agent.hpp>
#include <cgo/montecarlo/montecarlo_agent.hpp>
#include <cgo/minimax/minimax_agent.hpp>

using namespace cgo;
using namespace cgo::base;
using namespace cgo::driver;

Driver::Driver() :
   _state(), _turn(0)
{}

/* virtual */ Driver::~Driver() {
}

void Driver::play() {
   std::array< Agent*, 2 > agents = this->getPlayerAgents();
   std::array< boost::optional< Predecessor >, 2 > predecessors;
   std::array< bool, 2 > passes;

   predecessors.fill(boost::none);
   passes.fill(false);

   do {
      int playerIndex = this->_turn % 2;
      int opponentIndex = (this->_turn + 1) % 2;

      this->announceTurnStart();

      auto move = agents[playerIndex]->makeMove(this->_state,
       predecessors[opponentIndex]);
      predecessors[playerIndex] = std::make_tuple(move, this->_state);

      this->announceMove(move);

      Action* action = boost::get< Action >(&move);
      if (action != nullptr) {
         this->_state = State::applyAction(this->_state, *action);
         passes[playerIndex] = false;
      } else {
         passes[playerIndex] = true;
      }

      ++this->_turn;

      this->announceTurnEnd();
   } while (!passes[0] || !passes[1]);

   this->announceGameEnd();
}

Agent* Driver::choiceToAgent(int choice, Marker marker) const {
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
