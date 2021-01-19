#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// constructor
Process::Process(int pid) { 
    pid_ = pid; 
    User_ = "user";//LinuxParser::User(pid);
    CpuUtilization_ = LinuxParser::CpuUtilization(pid);
    RamUtilization_ = LinuxParser::Ram(pid);
    Uptime_ = LinuxParser::UpTime(pid);
    Command_ = LinuxParser::Command(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return User_; } 

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return CpuUtilization_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return RamUtilization_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return Uptime_; }

// TODO: Return the command that generated this process
string Process::Command() { return Command_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return this->CpuUtilization_ > a.CpuUtilization_; 
}
