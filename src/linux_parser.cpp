#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "linux_parser.h"
#include "file_manager.h"

using std::string;
using std::to_string;
using std::vector;
using std::unordered_map;
using std::stol;
using std::cout;
using std::endl;

StreamSingle ss;
StreamMultiple sm;

Stream::token_ids TokenIDs{};
Stream::token_values TokenValues;

long LinuxParser::to_long(string number) {
  // check for missing data 
  if (number == "") { number = "0";}

  // return string converted to long
  return stol(number, nullptr, 10);
}

// SYSTEM DATA

string LinuxParser::OperatingSystem() {
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  TokenValues = sm.GetData(kOSPath, TokenIDs, "key", "PRETTY_NAME", '=');  
  return TokenValues["value"];
}

string LinuxParser::Kernel() {
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(2, "kernel"));
  TokenValues = ss.GetData(kProcDirectory + kVersionFilename, TokenIDs);
  return TokenValues["kernel"];
}

void LinuxParser::CPU(unordered_map<string, string> & parms) {
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
  parms = sm.GetData(kProcDirectory + kStatFilename, TokenIDs, "key","cpu");   
}

float LinuxParser::MemoryUtilization() { 
  // return memory used / total memory, where memory used is //proc/meminfo MemTotal - MemFree
  long MemTotal;
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  TokenValues = sm.GetData(kProcDirectory + kMeminfoFilename, TokenIDs, "key", "MemTotal:");  
  MemTotal = to_long(TokenValues["value"]);

  long MemFree;
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  TokenValues = sm.GetData(kProcDirectory + kMeminfoFilename, TokenIDs, "key", "MemFree:");  
  MemFree = to_long(TokenValues["value"]);

  return float(MemTotal - MemFree) / float(MemTotal);
}

int LinuxParser::TotalProcesses() { 
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  TokenValues = sm.GetData(kProcDirectory + kStatFilename, TokenIDs, "key","processes");  
  return std::stoi(TokenValues["value"]); 
}

int LinuxParser::RunningProcesses() { 
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  TokenValues = sm.GetData(kProcDirectory + kStatFilename, TokenIDs, "key","procs_running");  
  return std::stoi(TokenValues["value"]); 
}

long LinuxParser::UpTime() {
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "uptime_system"));
  TokenValues = ss.GetData(kProcDirectory + kUptimeFilename, TokenIDs);
  return to_long(TokenValues["uptime_system"]);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }


// INDIVIDUAL PROCESSES
string LinuxParser::Uid(int pid) {  
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  TokenValues = sm.GetData(kProcDirectory + std::to_string(pid) + kStatusFilename, TokenIDs, "key", "Uid:");  
  return TokenValues["value"];
}

string LinuxParser::User(int pid) { 
  // use pid to retrieve uid, and uid to retrieve user name
  //return GetValueFilePasswd(Uid(pid));
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "user_name"));
  TokenIDs.push_back(std::make_pair(1, "x"));
  TokenIDs.push_back(std::make_pair(2, "user_id"));
  TokenValues = sm.GetData(kPasswordPath, TokenIDs, "user_id", to_string(pid), ':');  
  return TokenValues["user_name"];
}

float LinuxParser::CpuUtilization(int pid) { 
  // using algorithm: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(13, "utime"));
  TokenIDs.push_back(std::make_pair(14, "stime"));
  TokenIDs.push_back(std::make_pair(15, "cutime"));
  TokenIDs.push_back(std::make_pair(16, "cstime"));
  TokenIDs.push_back(std::make_pair(21, "starttime"));
  TokenValues = ss.GetData(kProcDirectory + std::to_string(pid) + kStatFilename, TokenIDs);

  // extract needed tokens
  long utime = to_long(TokenValues["utime"]);
  long stime = to_long(TokenValues["stime"]);
  long cutime = to_long(TokenValues["cutime"]);
  long cstime = to_long(TokenValues["cstime"]);
  long starttime = to_long(TokenValues["starttime"]);

  // get additional data
  long uptime = UpTime();  // of the system
  long Hertz = sysconf(_SC_CLK_TCK);  // clock ticks per second
  
  // calculation
  long total_time = utime + stime + cutime + cstime;
  float seconds = float(uptime) - float(starttime / Hertz);
  float cpu_usage = (float(total_time / Hertz) / seconds);
  return cpu_usage; 
}

string LinuxParser::Ram(int pid) { 
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "key"));
  TokenIDs.push_back(std::make_pair(1, "value"));
  TokenValues = sm.GetData(kProcDirectory + std::to_string(pid) + kStatusFilename, TokenIDs, "key", "VmSize:");  
  
  // cast ram in megabytes
  return to_string(to_long(TokenValues["value"]) / 1024);
}

long LinuxParser::UpTime(int pid) { 
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(21, "starttime"));
  TokenValues = ss.GetData(kProcDirectory + std::to_string(pid) + kStatFilename, TokenIDs);

  long starttime = to_long(TokenValues["starttime"]);

  // convert starttime to seconds 
  long Hertz = sysconf(_SC_CLK_TCK);  // clock ticks per second
  long UpTime = starttime / Hertz;
  
  return  UpTime;
}

string LinuxParser::Command(int pid) { 
  TokenIDs = {};
  TokenIDs.push_back(std::make_pair(0, "value"));
  TokenValues = ss.GetData(kProcDirectory + std::to_string(pid) + kCmdlineFilename, TokenIDs);
  
  if (TokenValues.count("value")) {
    return TokenValues["value"];
  } else {
    return "Value not found...";
  }
  
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }


// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);

  return pids;
}
