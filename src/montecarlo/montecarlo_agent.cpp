#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <time.h>
#include <ctime>
#include <cgo/montecarlo/montecarlo_agent.hpp>

using namespace std;
using namespace cgo::base;
using namespace cgo::montecarlo;

MonteCarloAgent::MonteCarloAgent(Marker marker) :
   Agent(marker), _turnNumber(0)
{}

/* virtual */ MonteCarloAgent::~MonteCarloAgent() {}

Marker cur_player = black;
boost::optional<tuple<Move, State>> predecessorPtr;
State m_state;
State tempState;
static const double UCTK = 0.44;
static const int numSims = 1000;

Node* MonteCarloAgent::getBestChild(Node* root) {
   Node* siblings = root->child;
   Node* best_child = NULL;
   double best_win = -1;
   while (siblings) {
      Position position(siblings->x, siblings->y);
      Action action(this->_marker, position);
      if (siblings->getWinRate() > best_win && m_state.isActionValid(action, predecessorPtr)) {
          best_child = siblings;
          best_win = siblings->getWinRate();
      }
      siblings = siblings->sibling;
   }
   return best_child;
}

Node* MonteCarloAgent::UCTSelect(Node* node) {
   Node* res = NULL;
   Node* next = node->child;
   double best_uct = 0;
   double uctvalue = 0;
   while (next != NULL) {
      if (next->visits > 0) {
         double winrate = next->getWinRate();
         double uct = UCTK * sqrt(log(node->visits) / (5 * next->visits));
         uctvalue = winrate + uct;
      }
      else {
         uctvalue = 10000 + (rand() % 1000);
      }
      if (uctvalue > best_uct) {
         best_uct = uctvalue;
         res = next;
      }
      next = next->sibling;
   }
   return res;
}

void MonteCarloAgent::createChildren(Node* parent, State& state,
 const boost::optional<tuple<Move, State>>& predecessor) {
   Node* last = parent;
   for (int i = 0; i < BOARD_DIMENSION; ++i) {
      for (int j = 0; j < BOARD_DIMENSION; ++j) {
         Position position(i, j);
         Action action(cur_player, position);
         if (state.isActionValid(action, predecessor)) {
            Node* n = new Node(i, j);
            if (last == parent) {
               last->child = n;
            }
            else {
               last->sibling = n;
            }
            last = n;
         }
      }
   }
}

bool MonteCarloAgent::checkGameOver(State& state, Move move,
 const boost::optional<tuple< Move, State>>& predecessor) {
   const Predecessor& predecessorTuple = predecessor.get();
   Move prevMove = get<0>(predecessorTuple);

   return (!boost::get<Action>(&move) && !boost::get<Action>(&prevMove)) ||
      state.getSuccessors(cur_player, predecessor).size() == 1;
}

Move MonteCarloAgent::makeRandomMove(State& state,
 const boost::optional< std::tuple< base::Move, base::State > >& predecessor) {
   vector<Successor> successors = state.getSuccessors(cur_player, predecessor);
   int index = 0;
   if (successors.size() == 1) {
      return Pass();
   }
   int size = successors.size() - 1;
   index = rand() % size;
   Move move = get<0>(successors.at(index));
   if (boost::get<Action>(&move)) {
      tempState = State::applyAction(state, boost::get<Action>(move));
      return move;
   }
   return Pass();
}

int MonteCarloAgent::playRandomGame(State& state,
 const boost::optional< std::tuple< base::Move, base::State > >& predecessor) {
   Move prevAction;
   Predecessor pred = predecessor.get();
   tempState = State(state);
   int moveCount = 0;
   do {
      prevAction = makeRandomMove(tempState, pred); // 0.0001
      cur_player == white ? cur_player = black : cur_player = white;
      pred = std::make_tuple(prevAction, tempState);
      ++moveCount;
   } while (!checkGameOver(tempState, prevAction, pred));
   //cout << moveCount << endl;
   if (get<0>(tempState.getScores()) > get<1>(tempState.getScores())) {
      return this->_marker == white ? 1 : 0;
   }
   else {
      return this->_marker == black ? 1 : 0;
   }
}

int randomresult = 0;
int MonteCarloAgent::playSimulation(Node* n, State& state,
  const boost::optional< std::tuple< base::Move, base::State > >& predecessor) {
   if (n->visits == 0) {
      randomresult = playRandomGame(state, predecessor); // 0.015
   }
   else {
      if (!n->child) {
         createChildren(n, state, predecessor);
      }
      Node *next = UCTSelect(n);

      Position position(next->x, next->y);
      Action action(cur_player, position);
      tempState = State::applyAction(state, action);
      cur_player == white ? cur_player = black : cur_player = white;
      State clone = State(tempState);
      Predecessor pred = make_tuple(action, clone);
      randomresult = playSimulation(next, clone, pred);
   }
   n->visits += 1;
   n->wins += randomresult;
   return randomresult;
}

bool MonteCarloAgent::makeSmartFirstMove(const State& state) const {
   Position p1(2, 2);
   return state.getBoard()[State::getIndex(p1)] == none;
}

Move MonteCarloAgent::makeMove(State& state,
 const boost::optional< std::tuple< Move, State > >& predecessor) {
   if (this->_turnNumber == 0) {
      ++this->_turnNumber;
      if (makeSmartFirstMove(state)) {
         Position p1(2, 2);
         return Action(this->_marker, p1);
      }
   }

   srand(time(NULL));
   Node *root = new Node(-2, -2);

   m_state = state;
   predecessorPtr = predecessor;
   cur_player = this->_marker;
   createChildren(root, state, predecessor); // 0.000131 seconds

   for (int i = 0; i < numSims; ++i) {
      State clone = State(state);
      // cout << "simulation " << i << endl;
      cur_player = this->_marker;
      playSimulation(root, clone, predecessor);
   }

   Node* n = getBestChild(root); // 0.000024
   Position position(n->x, n->y);

   if (n->getWinRate() == 0) {
      return Pass();
   }
   return Action(this->_marker, position);
}
