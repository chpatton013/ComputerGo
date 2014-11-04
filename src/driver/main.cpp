#include <cgo/driver/driver.hpp>
#include <cgo/driver/shell_driver.hpp>

using namespace cgo;

int main(int argc, char** argv) {
   driver::Driver* driver = new driver::ShellDriver();

   driver->play();

   delete driver;

   return 0;
}
