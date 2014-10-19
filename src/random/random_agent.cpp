#include <random>
#include <cgo/random/random_agent.hpp>

using namespace cgo::base;
using namespace cgo::random;

RandomAgent::RandomAgent(Marker marker) :
   Agent(marker)
{}

/* virtual */ RandomAgent::~RandomAgent() {}

Move RandomAgent::makeMove(State& state,
 const std::tuple< Move, State >& predecessor) {
   Move previousMove = std::get<0>(predecessor);
   Action* previousAction = boost::get< Action >(&previousMove);
   // Opponent passed last turn.
   if (previousAction == nullptr) {
      int myScore = state.getScore(this->_marker);
      int enemyScore = state.getScore(this->_opponentMarker);

      if (myScore > enemyScore) {
         return Pass();
      }
   }

   auto successors = state.getSuccessors(this->_marker, predecessor);
   int numSuccessors = successors.size();
   if (numSuccessors == 0) {
      return Pass();
   }

   std::random_device rd;
   auto randomSuccessor = successors[rd() % numSuccessors];
   return std::get<0>(randomSuccessor);
}
