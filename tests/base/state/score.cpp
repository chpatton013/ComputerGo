#include <catch.hpp>
#include <cgo/base/state.hpp>

using namespace cgo::base;

TEST_CASE("base/state/score/count/empty") {
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

   State state(board);

   std::tuple< int, int > expectedScore = std::make_tuple(0, 0);
   REQUIRE(state.getScores() == expectedScore);
}

TEST_CASE("base/state/score/count/white") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      W, n, n, n, n, n, n, n, n,
      W, n, n, n, n, n, n, n, n,
      W, n, n, n, n, n, n, n, n,
      n, n, W, n, n, n, n, n, n,
      n, n, W, n, n, n, n, n, n,
      n, n, W, n, n, n, n, n, n,
      n, n, n, n, W, n, n, n, n,
      n, n, n, n, W, n, n, n, n,
      n, n, n, n, W, n, n, n, n,
   }};

   State state(board);

   std::tuple< int, int > expectedScore = std::make_tuple(9, 0);
   REQUIRE(state.getScores() == expectedScore);
}

TEST_CASE("base/state/score/count/black") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
      B, n, n, n, n, n, n, n, n,
      B, n, n, n, n, n, n, n, n,
      B, n, n, n, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, B, n, n, n, n, n, n,
      n, n, n, n, B, n, n, n, n,
      n, n, n, n, B, n, n, n, n,
      n, n, n, n, B, n, n, n, n,
   }};

   State state(board);

   std::tuple< int, int > expectedScore = std::make_tuple(0, 9);
   REQUIRE(state.getScores() == expectedScore);
}

TEST_CASE("base/state/score/count/mixed") {
   auto n = none;
   auto W = white;
   auto B = black;

   State::Board board = {{
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

   State state(board);

   std::tuple< int, int > expectedScore = std::make_tuple(13, 11);
   REQUIRE(state.getScores() == expectedScore);
}

TEST_CASE("base/state/score/territory/white") {
}

TEST_CASE("base/state/score/territory/black") {
}

TEST_CASE("base/state/score/territory/mixed") {
}
