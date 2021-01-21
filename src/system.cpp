#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the operating system name
string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the system's kernel identifier (string)
string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }


// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    // remove existing processes
    processes_.clear();
    
    // get list of current process ids
    std::vector<int> pids = LinuxParser::Pids();
    
    // for each process id...
    for (int pid : pids) {
        // create a process object
        Process new_process(pid);
        
        // add the process object to the process list
        processes_.push_back(new_process);
    };

    // sort processes based on override of "less than" operator 
    std::sort(processes_.rbegin(), processes_.rend());
    
    return processes_; 
}
