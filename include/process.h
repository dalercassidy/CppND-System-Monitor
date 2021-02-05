#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // DONE: See src/process.cpp
  void Pid(int pid);
  
  std::string User();                      // DONE: See src/process.cpp
  void User(std::string user);

  std::string Command();                   // DONE: See src/process.cpp
  void Command(std::string command);

  float CpuUtilization();                  // DONE: See src/process.cpp
  void CpuUtilization(float cpuUtilization); 

  std::string Ram();                       // DONE: See src/process.cpp
  void Ram(std::string ram);

  long int UpTime();                       // DONE: See src/process.cpp
  void UpTime(long int upTime); 

  bool operator<(Process const& a) const;  // DONE: See src/process.cpp

  // DONE: Declare any necessary private members
 private:
    int pid_;
    std::string user_;
    std::string command_;
    float cpuUtilization_;
    std::string ram_;
    long int upTime_;

};

#endif