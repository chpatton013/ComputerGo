#pragma once

#include <tuple>
#include <cgo/driver/driver.hpp>

namespace cgo {
namespace driver {

class ShellDriver : public Driver {
public:
   ShellDriver();
   virtual ~ShellDriver();

protected:
   std::array< base::Agent*, 2 > getPlayerAgents() const;
   void announceTurnStart() const;
   void announceTurnEnd() const;
   void announceMove(const base::Move& move) const;
   void announceGameEnd() const;
};

} // namespace driver
} // namespace cgo
