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
   void announceTurn() const;
   void announceTurnEnd() const;
   void announceWinner() const;
};

} // namespace driver
} // namespace cgo
