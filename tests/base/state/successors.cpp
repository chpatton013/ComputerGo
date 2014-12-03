#include <catch.hpp>
#include <cgo/base/state.hpp>

using namespace cgo::base;

#include <stdio.h>
void assertSuccessors(const Board& board,
 const Board& expectedBoard, Marker marker,
 const boost::optional< Predecessor >& predecessor) {
   std::vector< Position > expected;
   for (int row = 0; row < BOARD_DIMENSION; ++row) {
      for (int col = 0; col < BOARD_DIMENSION; ++col) {
         int index = row * BOARD_DIMENSION + col;
         if (expectedBoard[index] == marker) {
            expected.push_back(Position(row, col));
         }
      }
   }

   State state(board);
   auto successors = state.getSuccessors(marker, predecessor);

   std::vector< Position > actual;
   for (auto successor : successors) {
      Move& move = std::get<0>(successor);
      Action* action = boost::get< Action >(&move);
      if (action == nullptr) {
         continue;
      }

      actual.push_back(action->position);
   }

   REQUIRE(actual == expected);
}

void assertSuccessors(const Board& board,
 const Board& expectedBoard, Marker marker) {
   assertSuccessors(board, expectedBoard, marker,
    boost::optional< Predecessor >());
}

// TEST_CASE("base/state/successors/empty") {
//    auto n = none;
//    auto W = white;
//    auto B = black;

//    Board board = {{
//       n, n, n, n, n, n, n, n, n,
//       n, n, n, n, n, n, n, n, n,
//       n, n, n, n, n, n, n, n, n,
//       n, n, n, n, n, n, n, n, n,
//       n, n, n, n, n, n, n, n, n,
//       n, n, n, n, n, n, n, n, n,
//       n, n, n, n, n, n, n, n, n,
//       n, n, n, n, n, n, n, n, n,
//       n, n, n, n, n, n, n, n, n,
//    }};

//    Board whiteExpectedBoard = {{
//       W, W, W, W, W, W, W, W, W,
//       W, W, W, W, W, W, W, W, W,
//       W, W, W, W, W, W, W, W, W,
//       W, W, W, W, W, W, W, W, W,
//       W, W, W, W, W, W, W, W, W,
//       W, W, W, W, W, W, W, W, W,
//       W, W, W, W, W, W, W, W, W,
//       W, W, W, W, W, W, W, W, W,
//       W, W, W, W, W, W, W, W, W,
//    }};
//    assertSuccessors(board, whiteExpectedBoard, white);

//    Board blackExpectedBoard = {{
//       B, B, B, B, B, B, B, B, B,
//       B, B, B, B, B, B, B, B, B,
//       B, B, B, B, B, B, B, B, B,
//       B, B, B, B, B, B, B, B, B,
//       B, B, B, B, B, B, B, B, B,
//       B, B, B, B, B, B, B, B, B,
//       B, B, B, B, B, B, B, B, B,
//       B, B, B, B, B, B, B, B, B,
//       B, B, B, B, B, B, B, B, B,
//    }};
//    assertSuccessors(board, blackExpectedBoard, black);
// }

// TEST_CASE("base/state/successors/white") {
//    auto n = none;
//    auto W = white;
//    auto B = black;

//    Board board = {{
//       W, W, W, W, W, W, W, W, W,
//       W, W, n, n, W, n, n, W, W,
//       W, n, W, n, W, n, W, n, W,
//       W, n, n, W, W, W, n, n, W,
//       W, W, W, W, W, W, W, W, W,
//       W, n, n, W, W, W, n, n, W,
//       W, n, W, n, W, n, W, n, W,
//       W, W, n, n, W, n, n, W, W,
//       W, W, W, W, W, W, W, W, W,
//    }};

//    Board whiteExpectedBoard = {{
//       n, n, n, n, n, n, n, n, n,
//       n, n, W, W, n, W, W, n, n,
//       n, W, n, W, n, W, n, W, n,
//       n, W, W, n, n, n, W, W, n,
//       n, n, n, n, n, n, n, n, n,
//       n, W, W, n, n, n, W, W, n,
//       n, W, n, W, n, W, n, W, n,
//       n, n, W, W, n, W, W, n, n,
//       n, n, n, n, n, n, n, n, n,
//    }};
//    assertSuccessors(board, whiteExpectedBoard, white);

//    Board blackExpectedBoard = {{
//       n, n, n, n, n, n, n, n, n,
//       n, n, B, B, n, B, B, n, n,
//       n, B, n, B, n, B, n, B, n,
//       n, B, B, n, n, n, B, B, n,
//       n, n, n, n, n, n, n, n, n,
//       n, B, B, n, n, n, B, B, n,
//       n, B, n, B, n, B, n, B, n,
//       n, n, B, B, n, B, B, n, n,
//       n, n, n, n, n, n, n, n, n,
//    }};
//    assertSuccessors(board, blackExpectedBoard, black);
// }

// TEST_CASE("base/state/successors/black") {
//    auto n = none;
//    auto W = white;
//    auto B = black;

//    Board board = {{
//       B, B, B, B, B, B, B, B, B,
//       B, B, n, n, B, n, n, B, B,
//       B, n, B, n, B, n, B, n, B,
//       B, n, n, B, B, B, n, n, B,
//       B, B, B, B, B, B, B, B, B,
//       B, n, n, B, B, B, n, n, B,
//       B, n, B, n, B, n, B, n, B,
//       B, B, n, n, B, n, n, B, B,
//       B, B, B, B, B, B, B, B, B,
//    }};

//    Board whiteExpectedBoard = {{
//       n, n, n, n, n, n, n, n, n,
//       n, n, W, W, n, W, W, n, n,
//       n, W, n, W, n, W, n, W, n,
//       n, W, W, n, n, n, W, W, n,
//       n, n, n, n, n, n, n, n, n,
//       n, W, W, n, n, n, W, W, n,
//       n, W, n, W, n, W, n, W, n,
//       n, n, W, W, n, W, W, n, n,
//       n, n, n, n, n, n, n, n, n,
//    }};
//    assertSuccessors(board, whiteExpectedBoard, white);

//    Board blackExpectedBoard = {{
//       n, n, n, n, n, n, n, n, n,
//       n, n, B, B, n, B, B, n, n,
//       n, B, n, B, n, B, n, B, n,
//       n, B, B, n, n, n, B, B, n,
//       n, n, n, n, n, n, n, n, n,
//       n, B, B, n, n, n, B, B, n,
//       n, B, n, B, n, B, n, B, n,
//       n, n, B, B, n, B, B, n, n,
//       n, n, n, n, n, n, n, n, n,
//    }};
//    assertSuccessors(board, blackExpectedBoard, black);
// }

// TEST_CASE("base/state/successors/mixed") {
//    auto n = none;
//    auto W = white;
//    auto B = black;

//    Board board = {{
//       B, B, B, B, W, B, B, B, B,
//       B, W, n, n, B, n, n, W, B,
//       B, n, W, n, W, n, W, n, B,
//       B, n, n, W, B, W, n, n, B,
//       W, B, W, B, W, B, W, B, W,
//       B, n, n, W, B, W, n, n, B,
//       B, n, W, n, W, n, W, n, B,
//       B, W, n, n, B, n, n, W, B,
//       B, B, B, B, W, B, B, B, W,
//    }};

//    Board whiteExpectedBoard = {{
//       n, n, n, n, n, n, n, n, n,
//       n, n, W, W, n, W, W, n, n,
//       n, W, n, W, n, W, n, W, n,
//       n, W, W, n, n, n, W, W, n,
//       n, n, n, n, n, n, n, n, n,
//       n, W, W, n, n, n, W, W, n,
//       n, W, n, W, n, W, n, W, n,
//       n, n, W, W, n, W, W, n, n,
//       n, n, n, n, n, n, n, n, n,
//    }};
//    assertSuccessors(board, whiteExpectedBoard, white);

//    Board blackExpectedBoard = {{
//       n, n, n, n, n, n, n, n, n,
//       n, n, B, B, n, B, B, n, n,
//       n, B, n, B, n, B, n, B, n,
//       n, B, B, n, n, n, B, B, n,
//       n, n, n, n, n, n, n, n, n,
//       n, B, B, n, n, n, B, B, n,
//       n, B, n, B, n, B, n, B, n,
//       n, n, B, B, n, B, B, n, n,
//       n, n, n, n, n, n, n, n, n,
//    }};
//    assertSuccessors(board, blackExpectedBoard, black);
// }
