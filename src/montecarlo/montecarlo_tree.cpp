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

int Node::update(int val) {
   this->visits++;
   
   this->wins += val;
   //std::cout << "Update : " << this->x << " " << this->y  << " " << this->visits << " " << this->wins << std::endl;
   return 0;
}

double Node::getWinRate() {
   if (this->visits > 0) {
      return (double)this->wins / this->visits;
   }
   else return 0;
}
