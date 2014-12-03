#include <iostream>
#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>
#include <cgo/base/types.hpp>
#include <cgo/driver/shell_driver.hpp>

using namespace cgo;
using namespace cgo::base;
using namespace cgo::driver;

ShellDriver::ShellDriver() :
   Driver()
{}

/* virtual */ ShellDriver::~ShellDriver() {
}

std::array< Agent*, 2 > ShellDriver::getPlayerAgents() const {
   int whiteChoice = -1;
   int blackChoice = -1;

   do {
      std::cout << "Select player type for player 1 (white):" << std::endl;
      std::cout << "   1: human controlled" << std::endl;
      std::cout << "   2: random AI" << std::endl;
      std::cout << "   3: montecarlo AI" << std::endl;
      std::cout << "   4: minimax AI" << std::endl;
      std::cout << "-> ";
      std::cin >> whiteChoice;
   } while (whiteChoice < 1 || whiteChoice > 4);

   do {
      std::cout << "Select player type for player 2 (black):" << std::endl;
      std::cout << "   1: human controlled" << std::endl;
      std::cout << "   2: random AI" << std::endl;
      std::cout << "   3: montecarlo AI" << std::endl;
      std::cout << "   4: minimax AI" << std::endl;
      std::cout << "-> ";
      std::cin >> blackChoice;
   } while (blackChoice < 1 || blackChoice > 4);

   std::cout << std::endl;

   Agent* whiteAgent = this->choiceToAgent(whiteChoice - 1,
    Marker::white);
   Agent* blackAgent = this->choiceToAgent(blackChoice - 1,
    Marker::black);
   return {{ whiteAgent, blackAgent }};
}

void ShellDriver::announceTurnStart() const {
   int globalTurn = this->_turn / 2 + 1;
   int playerIndex = this->_turn % 2 + 1;

   std::cout << "Turn " << globalTurn << ", Player " << playerIndex << std::endl;
   State::printBoard(this->_state);
}

void ShellDriver::announceTurnEnd() const {
   std::cout << std::endl;
}

void ShellDriver::announceMove(const Move& move) const {
   std::cout << "Move: ";

   const Pass* pass = boost::get< Pass >(&move);
   if (pass == nullptr) {
      Action action = boost::get< Action >(move);
      Position position = action.position;
      std::cout << (position.row + 1) << ", " << (position.col + 1) << std::endl;
   } else {
      std::cout << "pass" << std::endl;
   }
}

void ShellDriver::announceGameEnd() const {
   auto scores = this->_state.getScores();
   int whiteScore = std::get<0>(scores);
   int blackScore = std::get<1>(scores);

   std::cout << "Game over after " << this->_turn;
   if (this->_turn == 1) {
      std::cout << " turn" << std::endl;
   } else {
      std::cout << " turns" << std::endl;
   }

   std::cout << "   Player 1 (white): " << whiteScore << std::endl;
   std::cout << "   Player 2 (black): " << blackScore << std::endl;

   if (whiteScore > blackScore) {
      std::cout << "Player 1 Wins" << std::endl;
   } else if (whiteScore < blackScore) {
      std::cout << "Player 2 Wins" << std::endl;
   } else {
      std::cout << "Tie Game!" << std::endl;
   }
}
