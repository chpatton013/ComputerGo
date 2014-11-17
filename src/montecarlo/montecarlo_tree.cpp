#include <iostream>
#include <string>
#include <sstream>
#include <cgo/montecarlo/montecarlo_agent.hpp>

using namespace cgo::base;
using namespace cgo::standardin;

Node(Position p) {
   this.position = p;
}

int update(int val) {
   this.visits++;
   this.wins += val;
   return 0;
}

double getWinRate() {
   if (this.visits > 0) {
      return (double)this.wins / this.visits;
   }
   else return 0;
}
