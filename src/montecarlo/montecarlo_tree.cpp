#include <iostream>
#include <string>
#include <sstream>
#include <cgo/montecarlo/montecarlo_tree.hpp>

Node::Node(int x, int y) {
   this->x = x;
   this->y = y;
}

int Node::update(int val) {
   this->visits++;
   this->wins += val;
   return 0;
}

double Node::getWinRate() {
   if (this->visits > 0) {
      return (double)this->wins / this->visits;
   }
   else return 0;
}
