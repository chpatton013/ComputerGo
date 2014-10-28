#include <catch.hpp>
#include <cgo/base/state.hpp>

using namespace cgo::base;

void assertCapture(const State::Board& board, const Action& action,
 const State::Board& expected) {
   State::Board actual = State::applyAction(State(board), action).getBoard();
   REQUIRE(expected == actual);
}

TEST_CASE("base/state/capture/empty") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   Action action(white, Position(1, 1));

   State::Board expected = {{
      n, n, n, n, n, n, n, n, n,
      n, W, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   assertCapture(board, action, expected);
}

TEST_CASE("base/state/capture/empty/white") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      n, n, W, n, n, n, n, n, n,
      n, W, n, W, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   Action action(white, Position(2, 2));

   State::Board expected = {{
      n, n, W, n, n, n, n, n, n,
      n, W, n, W, n, n, n, n, n,
      n, n, W, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   assertCapture(board, action, expected);
}

TEST_CASE("base/state/capture/empty/black") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      n, n, B, n, n, n, n, n, n,
      n, B, n, B, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   Action action(black, Position(2, 2));

   State::Board expected = {{
      n, n, B, n, n, n, n, n, n,
      n, B, n, B, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   assertCapture(board, action, expected);
}

TEST_CASE("base/state/capture/white") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      n, n, W, n, n, n, n, n, n,
      n, W, B, W, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   Action action(white, Position(2, 2));

   State::Board expected = {{
      n, n, W, n, n, n, n, n, n,
      n, W, n, W, n, n, n, n, n,
      n, n, W, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   assertCapture(board, action, expected);
}

TEST_CASE("base/state/capture/black") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      n, n, B, n, n, n, n, n, n,
      n, B, W, B, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   Action action(black, Position(2, 2));

   State::Board expected = {{
      n, n, B, n, n, n, n, n, n,
      n, B, n, B, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   assertCapture(board, action, expected);
}

TEST_CASE("base/state/capture/contested") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      n, n, W, n, n, n, n, n, n,
      n, W, B, n, W, n, n, n, n,
      n, n, W, W, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   Action action(white, Position(0, 3));

   State::Board expected = {{
      n, n, W, W, n, n, n, n, n,
      n, W, B, n, W, n, n, n, n,
      n, n, W, W, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   assertCapture(board, action, expected);
}

TEST_CASE("base/state/capture/surrounded") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      n, n, W, n, n, n, n, n, n,
      n, W, B, B, W, n, n, n, n,
      n, n, W, W, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   Action action(white, Position(0, 3));

   State::Board expected = {{
      n, n, W, W, n, n, n, n, n,
      n, W, n, n, W, n, n, n, n,
      n, n, W, W, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   assertCapture(board, action, expected);
}

TEST_CASE("base/state/capture/one-eye") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      n, n, B, n, n, n, n, n, n,
      n, B, W, B, n, n, n, n, n,
      B, W, n, W, B, n, n, n, n,
      n, B, W, B, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   Action action(black, Position(2, 2));

   State::Board expected = {{
      n, n, B, n, n, n, n, n, n,
      n, B, n, B, n, n, n, n, n,
      B, n, B, n, B, n, n, n, n,
      n, B, n, B, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   assertCapture(board, action, expected);
}

TEST_CASE("base/state/capture/two-eyes") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      n, n, B, n, n, n, n, n, n,
      n, B, W, B, n, n, n, n, n,
      B, W, n, W, B, n, n, n, n,
      n, B, W, n, W, B, n, n, n,
      n, n, B, W, B, n, n, n, n,
      n, n, n, B, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   Action action1(black, Position(2, 2));
   Action action2(black, Position(3, 3));

   State::Board expected = {{
      n, n, B, n, n, n, n, n, n,
      n, B, W, B, n, n, n, n, n,
      B, W, n, W, B, n, n, n, n,
      n, B, W, n, W, B, n, n, n,
      n, n, B, W, B, n, n, n, n,
      n, n, n, B, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
      n, n, n, n, n, n, n, n, n,
   }};

   assertCapture(board, action1, expected);
   assertCapture(board, action2, expected);
}
