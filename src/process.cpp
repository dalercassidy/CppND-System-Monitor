#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
void Process::Pid(int pid) { pid_ = pid; }
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization_; }
void Process::CpuUtilization(float cpuUtilization) { cpuUtilization_ = cpuUtilization; }

// DONE: Return the command that generated this process
string Process::Command() { return command_; }
void Process::Command(std::string command) { command_ = command; }

// DONE: Return this process's memory utilization
string Process::Ram() { return ram_; }
void Process::Ram(std::string ram) { ram_ = ram; }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }
void Process::User(std::string user) { user_ = user; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }
void Process::UpTime(long int upTime) { upTime_ = upTime; } 
 
// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return this->cpuUtilization_ > a.cpuUtilization_;
}