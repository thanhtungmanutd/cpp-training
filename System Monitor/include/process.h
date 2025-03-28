#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"

using namespace LinuxParser;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
  public:
    Process(int pid);
    int Pid();                               // TODO: See src/process.cpp
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    float CpuUtilization();                  // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  // TODO: Declare any necessary private members
  private:
    int pid_;
    std::string user_;
    std::string cmd_;
    float cpu_utilization_;
    std::string ram_;
    long int up_time_;
};

#endif