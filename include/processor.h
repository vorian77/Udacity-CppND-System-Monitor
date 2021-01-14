#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  unordered_map<string, string> parms_prev, parms_current;
  long ParmPrev(string);
  long ParmCurrent(string);
};

#endif