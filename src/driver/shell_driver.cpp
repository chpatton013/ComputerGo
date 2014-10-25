#include <array>
#include <iostream>
#include <tuple>
#include <boost/optional.hpp>
#include <cgo/base/types.hpp>
#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>
#include <cgo/stdin/stdin_agent.hpp>
#include <cgo/random/random_agent.hpp>

using namespace cgo;

static std::tuple< int, int > getPlayerChoices() {
   int player1Choice = -1;
   int player2Choice = -1;

   do {
      std::cout << "Select player type for player 1 (white):" << std::endl;
      std::cout << "   1: human controlled" << std::endl;
      std::cout << "   2: random AI" << std::endl;
      std::cout << "-> ";
      std::cin >> player1Choice;
   } while (player1Choice < 1 || player1Choice > 2);

   do {
      std::cout << "Select player type for player 2 (black):" << std::endl;
      std::cout << "   1: human controlled" << std::endl;
      std::cout << "   2: random AI" << std::endl;
      std::cout << "-> ";
      std::cin >> player2Choice;
   } while (player2Choice < 1 || player2Choice > 2);

   std::cout << std::endl;

   return std::make_tuple(player1Choice - 1, player2Choice - 1);
}

static base::Agent* getAgentChoice(base::Marker marker, int choice) {
   switch (choice) {
   case 0:
      return new standardin::StdinAgent(marker);
   case 1:
      return new random::RandomAgent(marker);
   }

   return nullptr;
}

static void announceWinner(int turn, const std::tuple< int, int >& scores) {
   int whiteScore = std::get<0>(scores);
   int blackScore = std::get<1>(scores);

   std::cout << "Game over after " << turn;
   if (turn == 1) {
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

int main(int argc, char** argv) {
   std::tuple< int, int > choices = getPlayerChoices();

   std::array< base::Agent*, 2 > agents = {{
      getAgentChoice(base::Marker::white, std::get<0>(choices)),
      getAgentChoice(base::Marker::black, std::get<1>(choices)),
   }};

   base::State state;

   std::array< boost::optional< std::tuple< base::Move, base::State > >, 2 > predecessors;

   int turn = 0;
   std::array< bool, 2 > passes = {{
      false,
      false,
   }};

   do {
      int playerIndex = turn % 2;
      int opponentIndex = (turn + 1) % 2;

      std::cout << "Turn " << (turn / 2 + 1) << ", Player " << (playerIndex + 1) << std::endl;
      state.printBoard();

      boost::optional< std::tuple< base::Move, base::State > > opponentPredecessor = predecessors[opponentIndex];
      base::Move move = agents[playerIndex]->makeMove(state, opponentPredecessor);

      base::Action* action = boost::get< base::Action >(&move);
      if (action != nullptr) {
         state = base::State::applyAction(state, *action);
         passes[playerIndex] = false;
      } else {
         passes[playerIndex] = true;
      }

      opponentPredecessor = std::make_tuple(move, state);

      ++turn;
      std::cout << std::endl;
   } while (!passes[0] || !passes[1]);

   announceWinner(turn / 2, state.getScores());

   return 0;
}
