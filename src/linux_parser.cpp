#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::unordered_map;
using std::stoll;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

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

// return string converted to long
long LinuxParser::to_long(string number) {
  return stoll(number, nullptr, 10);
}

// Read data from file: /proc/stat
string LinuxParser::GetValueFileStat(string TargetKey) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == TargetKey) {
          return value;
        }
      }
    }
  }
  return value;
}

// Read first value of data from files with key followed by a colon, eg. Uid:
string LinuxParser::GetValueFileColon(string FileName, string TargetKey) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + FileName);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == TargetKey) {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  // return memory used as memory used / total memory, 
  // where memory used is //proc/meminfo MemTotal - MemFree
  long MemTotal, MemFree;

  MemTotal = to_long(GetValueFileColon(kMeminfoFilename, "MemTotal"));
  MemFree = to_long(GetValueFileColon(kMeminfoFilename, "MemFree"));

  return float(MemTotal - MemFree) / float(MemTotal);
}

// TODO: Read and return the system uptime from /proc/uptime
long LinuxParser::UpTime() {
  string uptime_system, uptime_idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_system >> uptime_idle;
  }
  return to_long(uptime_system);
}

// Read data from file: /proc/stat
bool LinuxParser::GetValueFileStatCPU(unordered_map<string, string> & parms) {
  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> parms["user"] >> parms["nice"] >> parms["system"] >> parms["idle"] >> parms["iowait"] >> parms["irq"] >> parms["softirq"] >> parms["steal"] >> parms["guest"]) {
        if (key == "cpu") {
          return true;
        }
      }
    }
  }
  return false;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return std::stoi(GetValueFileStat("processes")); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return std::stoi(GetValueFileStat("procs_running"));
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {  
  return GetValueFileColon(to_string(pid) + kStatusFilename, "Uid"); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  // use pid to retrieve uid
  string uid = Uid(pid);

  // use uid to retrieve user name
  return string(uid); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }