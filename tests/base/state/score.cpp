#include <catch.hpp>
#include <cgo/base/state.hpp>

using namespace cgo::base;

void assertScore(const Board& board,
 const std::tuple< int, int > expected) {
   State state(board);
   std::tuple< int, int > actual = state.getScores();
   REQUIRE(actual == expected);
}

TEST_CASE("base/state/score/count/empty") {
   auto n = none;
   auto W = white;
   auto B = black;

   Board board = {{
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

   std::tuple< int, int > expected = std::make_tuple(0, 0);

   assertScore(board, expected);
}

TEST_CASE("base/state/score/count/white") {
   auto n = none;
   auto W = white;
   auto B = black;

   Board board = {{
      W, n, n, n, n, n, n, n, B,
      W, n, n, n, n, n, n, n, n,
      W, n, n, n, n, n, n, n, n,
      n, n, W, n, n, n, n, n, n,
      n, n, W, n, n, n, n, n, n,
      n, n, W, n, n, n, n, n, n,
      n, n, n, n, W, n, n, n, n,
      n, n, n, n, W, n, n, n, n,
      n, n, n, n, W, n, n, n, n,
   }};

   std::tuple< int, int > expected = std::make_tuple(9, 1);

   assertScore(board, expected);
}

TEST_CASE("base/state/score/count/black") {
   auto n = none;
   auto W = white;
   auto B = black;

   Board board = {{
      B, n, n, n, n, n, n, n, W,
      B, n, n, n, n, n, n, n, n,
      B, n, n, n, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, n, n, B, n, n, n, n,
      n, n, n, n, B, n, n, n, n,
      n, n, n, n, B, n, n, n, n,
   }};

   std::tuple< int, int > expected = std::make_tuple(1, 9);

   assertScore(board, expected);
}

TEST_CASE("base/state/score/count/mixed") {
   auto n = none;
   auto W = white;
   auto B = black;

   Board board = {{
      W, n, n, n, n, n, n, n, n,
      W, B, n, n, n, n, n, n, n,
      W, B, W, n, n, n, n, n, n,
      n, B, W, B, n, n, n, n, n,
      n, n, W, B, W, n, n, n, n,
      n, n, n, B, W, B, n, n, n,
      n, n, n, n, W, B, W, n, n,
      n, n, n, n, n, B, W, B, n,
      n, n, n, n, n, n, W, B, W,
   }};

   std::tuple< int, int > expected = std::make_tuple(13, 11);

   assertScore(board, expected);
}

TEST_CASE("base/state/score/territory/white") {
   auto n = none;
   auto W = white;
   auto B = black;

   Board board = {{
      n, W, n, n, n, n, W, n, n,
      W, n, n, n, n, n, W, n, n,
      n, n, n, n, n, n, W, W, W,
      n, n, W, n, n, n, n, n, n,
      n, W, n, W, n, n, n, n, n,
      n, n, W, n, n, W, W, n, n,
      W, n, n, n, W, n, n, W, n,
      n, W, n, n, n, W, W, n, n,
      n, n, W, n, n, n, n, n, n,
   }};

   std::tuple< int, int > expected = std::make_tuple(81, 0);

   assertScore(board, expected);
}

TEST_CASE("base/state/score/territory/black") {
   auto n = none;
   auto W = white;
   auto B = black;

   Board board = {{
      n, B, n, n, n, n, B, n, n,
      B, n, n, n, n, n, B, n, n,
      n, n, n, n, n, n, B, B, B,
      n, n, B, n, n, n, n, n, n,
      n, B, n, B, n, n, n, n, n,
      n, n, B, n, n, B, B, n, n,
      B, n, n, n, B, n, n, B, n,
      n, B, n, n, n, B, B, n, n,
      n, n, B, n, n, n, n, n, n,
   }};

   std::tuple< int, int > expected = std::make_tuple(0, 81);

   assertScore(board, expected);
}

TEST_CASE("base/state/score/territory/mixed") {
   auto n = none;
   auto W = white;
   auto B = black;

   Board board = {{
      B, n, W, n, W, B, n, n, n,
      n, W, n, W, n, B, n, n, n,
      W, n, W, n, n, n, B, n, n,
      n, W, n, n, n, n, n, B, B,
      W, n, n, n, n, n, n, n, n,
      B, B, n, n, n, n, n, W, W,
      n, n, B, n, n, n, W, B, B,
      n, W, n, B, n, W, B, n, n,
      n, n, n, B, n, W, B, n, n,
   }};

   std::tuple< int, int > expected = std::make_tuple(18, 27);

   assertScore(board, expected);
}
