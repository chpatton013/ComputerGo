#include <catch.hpp>
#include <cgo/base/state.hpp>

using namespace cgo::base;

TEST_CASE("base/state/board/homogeneous") {
   Board emptyBoard;
   Board whiteBoard;
   Board blackBoard;

   emptyBoard.fill(none);
   whiteBoard.fill(white);
   blackBoard.fill(black);

   State emptyState(emptyBoard);
   State whiteState(whiteBoard);
   State blackState(blackBoard);

   for (int row = 0; row < BOARD_DIMENSION; ++row) {
      for (int col = 0; col < BOARD_DIMENSION; ++col) {
         REQUIRE(emptyState.getMarker(Position(row, col)) == none);
         REQUIRE(whiteState.getMarker(Position(row, col)) == white);
         REQUIRE(blackState.getMarker(Position(row, col)) == black);
      }
   }
}

TEST_CASE("base/state/board/mixed") {
   Board board;
   board.fill(none);
   for (int ndx = 0; ndx < BOARD_DIMENSION; ++ndx) {
      board[ndx * BOARD_DIMENSION] = white;
      board[ndx * BOARD_DIMENSION + BOARD_DIMENSION - 1] = black;
   }

   State mixedState(board);
   for (int row = 0; row < BOARD_DIMENSION; ++row) {
      REQUIRE(mixedState.getMarker(Position(row, 0)) == white);
      REQUIRE(mixedState.getMarker(Position(row, BOARD_DIMENSION - 1)) == black);
      for (int col = 1; col < BOARD_DIMENSION - 1; ++col) {
         REQUIRE(mixedState.getMarker(Position(row, col)) == none);
      }
   }
}
