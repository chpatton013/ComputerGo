#include <iostream>

int printBoard(State state) {
   std::cout << "  0 1 2 3 4 5 6 7 8\n";
   for (int ndxOuter = 0; ndxOuter < BOARD_DIMENSION; ++ndxOuter) {
      std::cout << ndxOuter << " "
      for (int ndxInner = 0; ndxInner < BOARD_DIMENSION; ndxInner++) {
         int index = ndxOuter * BOARD_DIMENSION + ndxInner;
         if (state.getBoard[index] == none) {
            std::cout << "-";
         } else if (state.getBoard[index] == black) {
            std::cout << "B";
         } else if (state.getBoard[index] == white) {
            std::cout << "W";
         }
      }
      std::cout << std::endl;
   }
}

int main() {
   int player1, player2;
   Move& move1, move2;
   Action* action1, action2;
   State state;

   Agent *agent1, *agent2;

   std::cout << "Player 1 is white, Player 2 is black" << std::endl;
   std::cout << "If you want a player to be human controlled, press 0 for that player" << std::endl;
   std::cout << "If you want a player to be AI controlled, press 1 for random agent" << std::endl;
   std::cout << "Input Player 1: ";
   std::cin >> player1;
   std::cout << "Input Player 2: ";
   std::cin >> player2;

   if (player1 == 0) {
      agent1 = new StdinAgent(white);
   } else {
      agent1 = new RandomAgent(white);
   }

   if (player2 == 0) {
      agent2 = new StdinAgent(black);
   } else {
      agent2 = new RandomAgent(black);
   }

   do {
      printBoard(state);
      move1 = agent1.makeMove();
      action1 = boost::get< Action >(&move1);
      state = State::applyAction(&state, action1);

      if (action1 != nullptr && action2 != nullptr) {
         break;
      }

      printBoard(state);
      move2 = agent2.makeMove();
      action2 = boost::get< Action >(&move2);
      state = State::applyAction(&state, action2);
   } while (action1 != nullptr && action2 != nullptr);

   std::tuple<int, int> score = state.getScores();
   int whiteScore = std::get<0>(score);
   int blackScore = std::get<1>(score);
   std::cout << "Player 1 " << whiteScore << std::endl;
   std::cout << "Player 2 " << blackScore << std::endl;
   if (whiteScore > blackScore) {
      std::cout << "Player 1, White Wins" << std::endl;
   } else if (whiteScore == blackScore) {
      std::cout << "Tie Game!" << std::endl;
   } else {
      std::cout << "Player 2, Black Wins!" << std::endl;
   }
}
