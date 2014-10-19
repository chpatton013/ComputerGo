#include <cgo/base/agent.hpp>
#include <cgo/base/state.hpp>

using namespace cgo::base;

Agent::Agent(Marker marker) :
   _marker(marker), _opponentMarker((this->_marker == white) ? black : white)
{
   State::validatePlayerMarker(this->_marker);
   State::validatePlayerMarker(this->_opponentMarker);
}

/* virtual */ Agent::~Agent() {}
