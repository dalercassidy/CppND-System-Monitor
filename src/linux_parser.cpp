#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <unistd.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memTotal, memFree;
  string memType, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> memType >> memTotal;
    std::getline(stream, line);
    linestream.str(line);
    linestream >> memType >> memFree;
  }

  return (memTotal - memFree) / memTotal;
  
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }  
  return uptime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return 0; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return 0; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  return 0; 
}

//Processor
float LinuxParser::SystemCpuUtilization() {
  string line, cpu;
  long user, nice, system, idle, iowait, irq, softirq, steal, Idle, NonIdle, Total;
  float systemCpuUtilization;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }

  Idle = idle + iowait;
  NonIdle = user + nice + system + irq + softirq + steal;
  Total = Idle + NonIdle;
  
  systemCpuUtilization = (Total - Idle) * 1.0 / Total;

  return systemCpuUtilization;

}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// DONE: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) { 
  string line;
  string value;
  long int cpuElement, upTime, startTime;
  float totalTime, cpuUtilization, seconds;

  upTime = LinuxParser::UpTime();

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= 13; i++) {
      linestream >> value;
    }
    for (int i = 14; i <= 15; i++)
    {
      linestream >> cpuElement;
      totalTime += 1.0 * cpuElement / sysconf(_SC_CLK_TCK); 
    }
    for (int i = 16; i <= 21; i++)
    {
      linestream >> value;       
    }
    linestream >> startTime;

    seconds = upTime - (1.0 * startTime / sysconf(_SC_CLK_TCK));

    cpuUtilization = 100 * totalTime / seconds;
  }
  return cpuUtilization; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {         
          return value;
        }
      }
    }
  }
  return value;  
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {         
          return value;
        }
      }
    }
  }
  return value; 
}


// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {   
  string line; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key; 
  long int value;
  string vmSize;
  
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {     
          vmSize = std::to_string(value / 1000);
        }
      }
    }
  }
  return vmSize;   
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {   
  string line;
  string key;
  string userid;
  
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> userid) {
        if (key == "Uid:") {         
          return userid;
        }
      }
    }
  }
  return userid; 
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {   
  string userid = LinuxParser::Uid(pid);
  
  string line;
  string key;
  string user;
  string delim = ":";
  string username, password, uid;
  int pos[3];
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      pos[0] = line.find(delim);
      pos[1] = line.find(delim, pos[0] + 1);
      pos[2] = line.find(delim, pos[1] + 1);      
      uid = line.substr(pos[1]+1, pos[2] - pos[1] - 1);   
      if (uid == userid) {         
        username = line.substr(0, pos[1] - 2);
        return username;
      }      
    }
  }
  return username; 
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, value;
  int upTime, processStartTime;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++) {
      linestream >> value;
    }
    linestream >> processStartTime;
    upTime = LinuxParser::UpTime() - processStartTime / sysconf(_SC_CLK_TCK);
    return upTime;  
  }
  return upTime; 
}