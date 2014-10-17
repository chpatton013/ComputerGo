#include <cgo/base/types.hpp>

using namespace cgo::base;

Position::Position(int r, int c) :
   row(r), col(c)
{}

/* virtual */ Position::~Position() {}

bool Position::operator==(const Position& rhs) const {
   return this->row == rhs.row && this->col == rhs.col;
}
