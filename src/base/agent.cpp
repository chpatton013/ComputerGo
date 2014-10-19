#include <cgo/base/agent.hpp>

using namespace cgo::base;

Agent::Agent(Marker marker) :
   _marker(marker)
{}

/* virtual */ Agent::~Agent() {}
