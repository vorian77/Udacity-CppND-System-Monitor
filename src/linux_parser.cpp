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


// return string converted to long
long LinuxParser::to_long(string number) {
  return stoll(number, nullptr, 10);
}

// return single line of data from a file, eg. /proc/[PID]/stat
string LinuxParser::GetFileLine(string FileName) {
  string line;
  std::ifstream filestream(FileName);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  } else {
    return NULL;
  }
}

// return vector of tokens from line of data with space " " as delimiter
// derived from: https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
vector <string> tokenize (string line) {
  vector <string> tokens; 
  
  // stringstream class check1 
  std::stringstream check1(line); 
    
  string intermediate; 
  
  while(getline(check1, intermediate, ' ')) 
  { 
      tokens.push_back(intermediate); 
  }
  return tokens; 
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

// Read data from file: /etc/passwd
string LinuxParser::GetValueFilePasswd(string TargetUid) {
  string line, name, x, uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> x >> uid) {
        if (uid == TargetUid) {
          return name;
        }
      }
    }
  }
  return name;
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



// SYSTEM DATA

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
// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  // return memory used as memory used / total memory, 
  // where memory used is //proc/meminfo MemTotal - MemFree
  long MemTotal, MemFree;

  MemTotal = to_long(GetValueFileColon(kMeminfoFilename, "MemTotal"));
  MemFree = to_long(GetValueFileColon(kMeminfoFilename, "MemFree"));

  return float(MemTotal - MemFree) / float(MemTotal);
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return std::stoi(GetValueFileStat("processes")); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return std::stoi(GetValueFileStat("procs_running"));
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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }


// INDIVIDUAL PROCESS DATA

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {  
  return GetValueFileColon(to_string(pid) + kStatusFilename, "Uid"); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  // use pid to retrieve uid, and uid to retrieve user name
  return GetValueFilePasswd(Uid(pid));
}

// TODO: Read and return CPU utilization of process
float LinuxParser::CpuUtilization(int pid) { 
  // using algorithm: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  
  // get all tokens from the process's stat file
  string FileName = kProcDirectory + to_string(pid) + kStatFilename;
  string line = GetFileLine(FileName);
  vector <string> tokens = tokenize(line);

  // extract needed tokens
  long utime = to_long(tokens[13]);
  long stime = to_long(tokens[14]);
  long cutime = to_long(tokens[15]);
  long cstime = to_long(tokens[16]);
  long starttime = to_long(tokens[21]);

  // get additional data
  long uptime = UpTime();
  long Hertz = sysconf(_SC_CLK_TCK);  // clock ticks per second
  
  // calculation
  long total_time = utime + stime + cutime + cstime;
  float seconds = float(uptime) - float(starttime / Hertz);
  float cpu_usage = 100 * (float(total_time / Hertz) / seconds);
  return cpu_usage; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
int LinuxParser::Ram(int pid) { 
  string RamInKb = GetValueFileColon(to_string(pid) + kStatusFilename, "VmSize");
  
  // cast ram in megabytes
  return to_long(RamInKb) / 1000;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }
