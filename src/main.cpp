#include "ncurses_display.h"
#include "system.h"

// temp
#include <vector>
#include <iostream>
#include "process.h"
#include "file_manager.h"

using std::cout;
using std::endl;

int main0() {
  System system;
  NCursesDisplay::Display(system);
}

int main() {
  const std::string kProcDirectory{"/proc/"};
  const std::string kCmdlineFilename{"/cmdline"};
  const std::string kCpuinfoFilename{"/cpuinfo"};
  const std::string kStatusFilename{"/status"};
  const std::string kStatFilename{"/stat"};
  const std::string kUptimeFilename{"/uptime"};
  const std::string kMeminfoFilename{"/meminfo"};
  const std::string kVersionFilename{"/version"};
  const std::string kOSPath{"/etc/os-release"};
  const std::string kPasswordPath{"/etc/passwd"};

  StreamSingle ss;
  StreamMultiple sm;

  Stream::token_ids TokenIDs{};
  Stream::token_values TokenValues;

// begin column idx at 0
  // SYSTEM

  // Operating System
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  sm.GetData(kOSPath, TokenIDs, "key", "PRETTY_NAME", '=');  

  // kernel
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(2, "kernel"));
  ss.GetData(kProcDirectory + kVersionFilename, TokenIDs);

  // CPU Utilization
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "user"));
  TokenIDs.push_back(std::make_pair(2, "nice"));
  TokenIDs.push_back(std::make_pair(3, "system"));
  TokenIDs.push_back(std::make_pair(4, "idle"));
  TokenIDs.push_back(std::make_pair(5, "iowait"));
  TokenIDs.push_back(std::make_pair(6, "irq"));
  TokenIDs.push_back(std::make_pair(7, "softirq"));
  TokenIDs.push_back(std::make_pair(8, "steal"));
  TokenIDs.push_back(std::make_pair(9, "guest"));
  TokenIDs.push_back(std::make_pair(10, "guest_nice"));
  sm.GetData(kProcDirectory + kStatFilename, TokenIDs, "key","cpu");  

  // memory utilization - MemTotal
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  sm.GetData(kProcDirectory + kMeminfoFilename, TokenIDs, "key", "MemTotal:");  

  // memory utilization - MemFree
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  sm.GetData(kProcDirectory + kMeminfoFilename, TokenIDs, "key", "MemFree:");  

  // Total Processes
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  sm.GetData(kProcDirectory + kStatFilename, TokenIDs, "key","processes");  

  // Running Processes
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  sm.GetData(kProcDirectory + kStatFilename, TokenIDs, "key","procs_running");  

  // UpTime
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "uptime_system"));
  ss.GetData(kProcDirectory + kUptimeFilename, TokenIDs);
  

  // PROCESSES
  int pid {1793};

  //Uid
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  TokenValues = sm.GetData(kProcDirectory + std::to_string(pid) + kStatusFilename, TokenIDs, "key", "Uid:");  
  string Uid = TokenValues["value"];

  // user name
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "user_name"));
  TokenIDs.push_back(std::make_pair(1, "x"));
  TokenIDs.push_back(std::make_pair(2, "user_id"));
  sm.GetData(kPasswordPath, TokenIDs, "user_id", Uid, ':');  

  // CPU Utilization
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(13, "utime"));
  TokenIDs.push_back(std::make_pair(14, "stime"));
  TokenIDs.push_back(std::make_pair(15, "cutime"));
  TokenIDs.push_back(std::make_pair(16, "cstime"));
  TokenIDs.push_back(std::make_pair(21, "starttime"));
  ss.GetData(kProcDirectory + std::to_string(pid) + kStatFilename, TokenIDs);

  // RAM Utilization
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  sm.GetData(kProcDirectory + std::to_string(pid) + kStatusFilename, TokenIDs, "key", "VmSize:");  

  // UpTime
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(21, "starttime"));
  ss.GetData(kProcDirectory + std::to_string(pid) + kStatFilename, TokenIDs);

  // Command
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "value"));
  ss.GetData(kProcDirectory + std::to_string(pid) + kCmdlineFilename, TokenIDs);

} 
