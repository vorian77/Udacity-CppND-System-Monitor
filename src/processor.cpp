#include <string>
#include <vector>
#include <unordered_map>
#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;
using std::unordered_map;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;
    float CPU_Percentage;

    // swap current and prev parms
    if (parms_current.size() > 0) {
        parms_prev = parms_current;
    }

    // get latest parms
    if (LinuxParser::GetValueFileStatCPU(parms_current)) {  
        if (parms_prev.size() > 0) {
            // compute using algorithm by Vangelis Tasoulas
            // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
            PrevIdle = ParmPrev("idle") + ParmPrev("iowait"); 
            Idle = ParmCurrent("idle") + ParmCurrent("iowait"); 
            
            PrevNonIdle = ParmPrev("user") + ParmPrev("nice") + ParmPrev("system") + ParmPrev("irq") + ParmPrev("softirq") + ParmPrev("steal");
            NonIdle = ParmCurrent("user") + ParmCurrent("nice") + ParmCurrent("system") + ParmCurrent("irq") + ParmCurrent("softirq") + ParmCurrent("steal");
            
            PrevTotal = PrevIdle + PrevNonIdle;
            Total = Idle + NonIdle;

            totald = Total - PrevTotal;
            idled = Idle - PrevIdle;

            CPU_Percentage = float(totald - idled) / float(totald);

            return CPU_Percentage;
        } else {
            return 0;
        };
    } else {
        // unable to retrive current parms
        return 0.0;
    };
}

// helper functions that cast parm values as long
long Processor::ParmPrev(string parm_name) { return LinuxParser::to_long(parms_prev[parm_name]); }
long Processor::ParmCurrent(string parm_name) { return LinuxParser::to_long(parms_current[parm_name]); }
