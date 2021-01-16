#include "ncurses_display.h"
#include "system.h"

// temp
#include <vector>
#include <iostream>
#include "process.h"

int main() {
  System system;
  
  // std::vector<Process>& processes = system.Processes();
  // std::cout << "p cnt: " << processes.size() << std::endl;

  // for (auto p: processes) {
  //   std::cout << "p id: " << p.Pid() << " ram: " << p.Ram() << " CPU: " << p.CpuUtilization() << std::endl;

  // };

  NCursesDisplay::Display(system);

  
}