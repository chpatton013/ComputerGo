#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <time.h>
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
static const double UCTK = 0.5;
static const int numSims = 1000;
Node* root = new Node(-1, -1);

Node* MonteCarloAgent::getBestChild() {
   Node* siblings = root->child;
   Node* best_child = NULL;
   double best = -1;
   while (siblings) {
      Position position(siblings->x, siblings->y);
      Action action(this->_marker, position);
      if (siblings->visits > best && m_state.isActionValid(action, predecessorPtr)) {
         best_child = siblings;
         best = siblings->visits;
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
   double winrate = 0;
   double uct = 0;
   while (next) {
      if (next->visits > 0) {
         winrate = next->getWinRate();
         uct = UCTK * sqrt(log(node->visits) / (5 * next->visits));
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
   int size = successors.size();
   while (size != 1) {
      int index = (rand() % size);
      Move move = get<0>(successors.at(index));
      if (boost::get<Action>(&move)) {
         tempState = State::applyAction(state, boost::get<Action>(move));
         return move;
      }
   }
   return Pass();
}

int MonteCarloAgent::playRandomGame(State& state,
 const boost::optional< std::tuple< base::Move, base::State > >& predecessor) {
   Move prevAction;
   Predecessor pred = predecessor.get();
   tempState = State(state);
   do {
      State clone = State(tempState);
      prevAction = makeRandomMove(clone, pred); // 0.0001
      cur_player == white ? cur_player = black : cur_player = white;
      pred = make_tuple(prevAction, clone);
   } while (!checkGameOver(tempState, prevAction, pred));
   if (get<0>(tempState.getScores()) > get<1>(tempState.getScores())) {
      return this->_marker == white ? 1 : 0;
   }
   else {
      return this->_marker == black ? 1 : 0;
   }
}

int MonteCarloAgent::playSimulation(Node* n, State& state,
  const boost::optional< std::tuple< base::Move, base::State > >& predecessor) {
   int randomresult = 0;
   if (n->visits == 0) {
      randomresult = playRandomGame(state, predecessor); // 0.015
   }
   else {
      Node *next;
      if (!n->child) {
         createChildren(n, state, predecessor);
      }
      if (!n->child) {
         next = n;
      }
      else {
         next = UCTSelect(n);
      }

      Position position(next->x, next->y);
      Action action(cur_player, position);
      tempState = State::applyAction(state, action);
      cur_player == white ? cur_player = black : cur_player = white;
      Predecessor pred = make_tuple(action, state);
      randomresult = playSimulation(next, tempState, pred);
   }
   n->visits++;
   n->wins += randomresult;
   return randomresult;
}

Move MonteCarloAgent::makeMove(State& state,
 const boost::optional< std::tuple< Move, State > >& predecessor) {
   if (this->_turnNumber == 0) {
      srand(time(NULL));
      ++this->_turnNumber;
      Position p1(2, 2);
      if (state.getBoard()[State::getIndex(p1)] == none) {
         return Action(this->_marker, p1);
      }
   }
   if (!boost::get<Action>(&(get<0>(predecessor.get())))) {
      int score = get<0>(state.getScores()) - get<1>(state.getScores());
      if (score >= 0) {
         if (this->_marker == white) {
            return Pass();
         }
      }
      else {
         if (this->_marker == black) {
            return Pass();
         }
      }
   }

   m_state = state;
   predecessorPtr = predecessor;
   cur_player = this->_marker;
   createChildren(root, state, predecessor); // 0.000131 seconds

   for (int i = 0; i < numSims; ++i) {
      // cout << "simulation " << i << endl;
      cur_player = this->_marker;
      playSimulation(root, state, predecessor);
   }

   Node* n = getBestChild(); // 0.000024
   Position position(n->x, n->y);

   return Action(this->_marker, position);
}
