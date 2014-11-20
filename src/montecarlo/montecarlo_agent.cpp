#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <cgo/montecarlo/montecarlo_agent.hpp>

using namespace std;
using namespace cgo::base;
using namespace cgo::montecarlo;

MonteCarloAgent::MonteCarloAgent(Marker marker) :
   Agent(marker)
{}

/* virtual */ MonteCarloAgent::~MonteCarloAgent() {}

Node * global_root;
int cur_player=-1;
boost::optional< std::tuple< Move, State > > predecessorPtr;
State m_state;
static const double UCTK = sqrt(1/5);
static const int numSims = 55;

int MonteCarloAgent::CalculateBest(Position position) {
   return 0;
}


Node MonteCarloAgent::getBestChild(Node* root) {
   Node* child = root->child;
   Node* best_child = NULL;
   double best_visits= -1;
   while (child) { // for all children
      Position position(child->x, child->y);
      Action action(this->_marker, position);
      //cout << child->x << " "<< child->y << " " << child->wins << " " <<  child->visits << " " <<  child->getWinRate() << endl;
      if (child->visits > best_visits && m_state.isActionValid(action, predecessorPtr)) {
          best_child=child;
          best_visits=child->visits;
      }
      child = child->sibling;
   }
   return *best_child;
}


Node *root;
//state -> construct new state and pass the board , use applyAction, state

Node MonteCarloAgent::UCTSelect(Node* node) {
   Node* res = NULL;
   Node* next = node->child;
   double best_uct = 0;
   while (next != NULL) {
      double uctvalue = 0;
      if (next->visits > 0) {
         double winrate = next->getWinRate();
         double uct = UCTK * sqrt(log(node->visits)/ next->visits);
         uctvalue = winrate + uct;
      }
      else {
         uctvalue = 10000 + 1000 * rand();
      }

      if (uctvalue > best_uct) {
         best_uct = uctvalue;
         res = next;
      }

      next = next->sibling;
   }

   return *res;
}


Action* MonteCarloAgent::makeRandomMove(State& state) {
   int x=0;
   int y=0;
   Action* action_ptr; //if a pass is also a move, may need this
   while (true) {     
      x = rand() % BOARD_SIZE;
      y = rand() % BOARD_SIZE;
      Position position(x, y);
      Action action(this->_marker, position);
      if (m_state.isActionValid(action, predecessorPtr)) {
         break;
      }
   }
   Position position(x, y);
   Action action(this->_marker, position);
   action_ptr = &action;
   state = State::applyAction(state, action);
   return action_ptr;
}



int MonteCarloAgent::createChildren(Node* parent) {
   Node* last=parent;
   int i = 0;
   int j = 0;
   for (i=0; i<BOARD_SIZE; i++) {
      for (j=0; j<BOARD_SIZE; j++) {
         Position position(i, j);
         Action action(this->_marker, position);
         if (m_state.isActionValid(action, predecessorPtr)) {
            Node* n = new Node(i, j);
            if (last==parent) {
               last->child= n;
               last = last->child;
            }
            else {
               last->sibling= n;
               last=last->sibling;
            }
            
            
         }
      }
   }
   return 0;
 }

bool MonteCarloAgent::checkGameOver(State& state, Action* action, 
 const boost::optional< std::tuple< Move, State > >& predecessor) {
   const Predecessor& predecessorTuple = predecessor.get();
   Move prevMove = std::get< 0 >(predecessorTuple);

   if (( !action && !boost::get<Action>(&prevMove) ) ||
    state.getSuccessors(this->_marker, predecessor).empty()) { //Both Action is Pass or If no possible moves left
      return true;
   }
   else {
      return false;
   }

}

// NEED TO FIX RANDOM GAME, doesn't stop playing so had to stop with a count
int MonteCarloAgent::playRandomGame(State& state) {
   Action* prevAction;
   Marker temp = this->_marker;
   Predecessor pred;
   int count = 0;
   do {
      prevAction = makeRandomMove(state);
      this->_marker = (this->_marker == white) ? black : white; // switch players
      pred = std::make_tuple(*prevAction, state);
      count++;
   } while (!checkGameOver(state, prevAction, pred) && count <= 110); //Allow first move
   this->_marker = temp;
   return ( std::get<0>(state.getScores()) > std::get<1>(state.getScores()) ) ? (cur_player == 0 ? 0 : 1) : (cur_player == 1 ? 0 : 1) ; //if black wins, if cur_player is black, then they win, else return white win
}

int MonteCarloAgent::playSimulation(Node* n, State& state) {
   Node* cursor;
   int randomresult = 0;
   if (n->child == NULL && n->visits < 10) {
      randomresult = playRandomGame(state);
   }
   else {
      if (!n->child) {
         createChildren(n);
      }
      Node next = UCTSelect(n);

      Position position(next.x, next.y);
      Action action(this->_marker, position);
      state = State::applyAction(state, action);
      this->_marker = (this->_marker == white) ? black : white; 

      int res = playSimulation(&next, state);
      randomresult = 1 - res;
   }
   n->update(1-randomresult); 
   cursor = global_root->child;

   // for some reason the node->update doesn't actually save the node values...
   while(cursor) {
      if(cursor->x == n->x && cursor->y == n->y) {
         cursor->wins += n->wins;
         cursor->visits += n->visits;
         break;
      }
      else {
         cursor = cursor->sibling;
      }

     
   }
   return randomresult; 
}


Move MonteCarloAgent::makeMove(State& state,
 const boost::optional< std::tuple< Move, State > >& predecessor) {
   int i = 0;
   Node temp = Node(-1, -1);
   Node* root = &temp;
   global_root = root;

   createChildren(root);
   m_state = state;
   State clone = State(state);
   Board boardClone;
   predecessorPtr = predecessor;
   Marker temp_marker = this->_marker;

   for (i = 0; i < numSims; i++) {
      boardClone = clone.getBoard();
      playSimulation(root, clone);
   }

   this->_marker = temp_marker;
   Node n = getBestChild(root);
   Position position(n.x, n.y);
   cout << n.x << " "<< n.y <<endl;
   return Action(this->_marker, position);
}

   // do {
   //    std:://cout << "Enter a move: <row> <col> or (p)ass: ";

   //    std::string move;
   //    do {
   //       std::getline(std::cin, move);
   //    } while (move.size() == 0);

   //    if (move == "p" || move == "pass") {
   //       return Pass();
   //    }

   //    int row, col;
   //    std::stringstream ss(move);
   //    ss >> row >> col;

   //    position = Position(row - 1, col - 1);
   // } while (!state.isActionValid(Action(this->_marker, position), predecessor));



  // return Action(this->_marker, position);
//}
