#include <catch.hpp>
#include <cgo/base/state.hpp>

using namespace cgo::base;

void assertLiberties(const Board& board, const Board& expected) {
   State state(board);
   std::vector< Position > liberties;
   Board actual;
   actual.fill(none);

   liberties = state.getLiberties(white);
   for (Position pos : liberties) {
      int index = pos.row * BOARD_DIMENSION + pos.col;
      REQUIRE(actual[index] == none);
      actual[index] = white;
   }

   liberties = state.getLiberties(black);
   for (Position pos : liberties) {
      int index = pos.row * BOARD_DIMENSION + pos.col;
      REQUIRE(actual[index] == none);
      actual[index] = black;
   }

   REQUIRE(actual == expected);
}

// TEST_CASE("base/state/liberties/empty") {
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

//    Board expected = {{
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

//    assertLiberties(board, expected);
// }

// TEST_CASE("base/state/liberties/white") {
//    auto n = none;
//    auto W = white;
//    auto B = black;

//    Board board = {{
//       W, n, n, n, W, W, n, n, W,
//       n, n, n, n, W, W, n, n, W,
//       n, n, n, n, n, n, n, n, W,
//       n, n, n, n, n, n, n, n, W,
//       n, n, n, n, n, n, W, W, n,
//       n, n, n, n, n, W, n, n, n,
//       W, W, W, W, n, W, n, n, n,
//       n, n, n, n, n, W, n, W, n,
//       W, W, W, W, W, W, W, n, n,
//    }};

//    Board expected = {{
//       n, W, n, W, n, n, W, W, n,
//       W, n, n, W, n, n, W, W, n,
//       n, n, n, n, W, W, n, W, n,
//       n, n, n, n, n, n, W, W, n,
//       n, n, n, n, n, W, n, n, W,
//       W, W, W, W, W, n, W, W, n,
//       n, n, n, n, W, n, W, W, n,
//       W, W, W, W, W, n, W, n, W,
//       n, n, n, n, n, n, n, W, n,
//    }};

//    assertLiberties(board, expected);
// }

// TEST_CASE("base/state/liberties/black") {
//    auto n = none;
//    auto W = white;
//    auto B = black;

//    Board board = {{
//       B, n, n, n, B, B, n, n, B,
//       n, n, n, n, B, B, n, n, B,
//       n, n, n, n, n, n, n, n, B,
//       n, n, n, n, n, n, n, n, B,
//       n, n, n, n, n, n, B, B, n,
//       n, n, n, n, n, B, n, n, n,
//       B, B, B, B, n, B, n, n, n,
//       n, n, n, n, n, B, n, B, n,
//       B, B, B, B, B, B, B, n, n,
//    }};

//    Board expected = {{
//       n, B, n, B, n, n, B, B, n,
//       B, n, n, B, n, n, B, B, n,
//       n, n, n, n, B, B, n, B, n,
//       n, n, n, n, n, n, B, B, n,
//       n, n, n, n, n, B, n, n, B,
//       B, B, B, B, B, n, B, B, n,
//       n, n, n, n, B, n, B, B, n,
//       B, B, B, B, B, n, B, n, B,
//       n, n, n, n, n, n, n, B, n,
//    }};

//    assertLiberties(board, expected);
// }

// TEST_CASE("base/state/liberties/mixed") {
//    auto n = none;
//    auto W = white;
//    auto B = black;

//    Board board = {{
//       W, B, n, n, n, n, n, n, n,
//       B, n, n, n, n, n, W, B, n,
//       n, n, n, n, B, n, n, n, n,
//       n, n, n, B, W, B, n, n, n,
//       n, n, n, n, B, n, n, n, n,
//       n, B, n, n, n, n, n, n, n,
//       B, n, B, n, n, n, n, n, n,
//       B, n, B, n, n, n, n, W, W,
//       n, B, n, n, n, n, n, W, B,
//    }};

//    Board expected = {{
//       n, n, B, n, n, n, W, B, n,
//       n, B, n, n, B, W, n, n, B,
//       B, n, n, B, n, B, W, B, n,
//       n, n, B, n, n, n, B, n, n,
//       n, B, n, B, n, B, n, n, n,
//       B, n, B, n, B, n, n, n, n,
//       n, B, n, B, n, n, n, W, W,
//       n, B, n, B, n, n, W, n, n,
//       B, n, B, n, n, n, W, n, n,
//    }};

//    assertLiberties(board, expected);
// }

// TEST_CASE("base/state/liberties/overlap") {
//    auto n = none;
//    auto W = white;
//    auto B = black;

//    Board board = {{
//       n, n, n, W, n, B, n, n, n,
//       n, n, n, W, n, B, n, n, n,
//       n, n, n, W, n, B, n, n, n,
//       n, n, n, W, n, B, n, n, n,
//       n, n, n, W, n, B, n, n, n,
//       n, n, n, W, n, B, n, n, n,
//       n, n, n, W, n, B, n, n, n,
//       n, n, n, W, n, B, n, n, n,
//       n, n, n, W, n, B, n, n, n,
//    }};

//    State state(board);

//    std::vector< Position > actual;
//    std::vector< Position > expected;

//    expected = {{
//       Position(0, 2),
//       Position(0, 4),
//       Position(1, 2),
//       Position(1, 4),
//       Position(2, 2),
//       Position(2, 4),
//       Position(3, 2),
//       Position(3, 4),
//       Position(4, 2),
//       Position(4, 4),
//       Position(5, 2),
//       Position(5, 4),
//       Position(6, 2),
//       Position(6, 4),
//       Position(7, 2),
//       Position(7, 4),
//       Position(8, 2),
//       Position(8, 4),
//    }};
//    actual = state.getLiberties(white);
//    REQUIRE(actual == expected);

//    expected = {{
//       Position(0, 4),
//       Position(0, 6),
//       Position(1, 4),
//       Position(1, 6),
//       Position(2, 4),
//       Position(2, 6),
//       Position(3, 4),
//       Position(3, 6),
//       Position(4, 4),
//       Position(4, 6),
//       Position(5, 4),
//       Position(5, 6),
//       Position(6, 4),
//       Position(6, 6),
//       Position(7, 4),
//       Position(7, 6),
//       Position(8, 4),
//       Position(8, 6),
//    }};
//    actual = state.getLiberties(black);
//    REQUIRE(actual == expected);
// }
