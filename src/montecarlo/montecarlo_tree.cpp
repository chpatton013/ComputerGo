#include <iostream>
#include <string>
#include <sstream>
#include <cgo/montecarlo/montecarlo_tree.hpp>

Node::Node(int x, int y) {
   this->x = x;
   this->y = y;
   this->visits = 0;
   this->wins = 0;
   this->child = NULL;
   this->sibling = NULL;
}

double Node::getWinRate() {
   if (this->visits > 0) {
      return (double)this->wins / this->visits;
   }
   else return 0;
}
