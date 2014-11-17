#pragma once

#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>
#include <cgo/base/types.hpp>

class Node;

class Node {
public:
   int visits = 0;
   int wins = 0;
   int x = -1;
   int y = -1;
   Node *child;
   Node *sibling;
   Node (int x, int y);
   int update (int val);
   double getWinRate ();
private:
};
