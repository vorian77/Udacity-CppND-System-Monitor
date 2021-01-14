#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

// helper division function
void divide(long divisor, long dividend, long & quotient, long & remainder) {
    quotient = dividend / divisor;
    remainder = dividend % divisor;
}

string Format::ElapsedTime(long seconds) { 
    long divisor, dividend, quotient, remainder;
    string rtn;

    // compute hours
    divisor = 3600;
    dividend = seconds;
    divide(divisor, dividend, quotient, remainder);
    rtn = to_string(quotient);

    // compute minutes & minutes
    divisor = 60;
    dividend = remainder;
    divide(divisor, dividend, quotient, remainder);
    rtn = rtn + ":" + to_string(quotient) + ":" + to_string(remainder) + " ";

   return string(rtn); 
}