#include <string>
#include <iomanip>
#include <iostream>

#include "format.h"

using std::string;


// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string HH, MM, SS;    

    HH = std::to_string(seconds / 3600);
    HH.insert(HH.begin(), 2 - HH.length(), '0');

    MM = std::to_string((seconds % 3600) / 60);  
    MM.insert(MM.begin(), 2 - MM.length(), '0');

    SS = std::to_string((seconds % 3600) % 60);
    SS.insert(SS.begin(), 2 - SS.length(), '0');

    return HH.append(":").append(MM).append(":").append(SS);

}