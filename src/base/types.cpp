#include <cgo/base/types.hpp>

using namespace cgo::base;

Position::Position(int _x, int _y) :
   x(_x), y(_y)
{}

/* virtual */ Position::~Position() {}
