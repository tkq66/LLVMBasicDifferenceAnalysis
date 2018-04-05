#include <string>
#include "SeparationTracker.h"

SeparationTracker::SeparationTracker(std::string varNameOne, std::string varNameTwo) {
    variablesTracker = {
        { variableNameOne, std::nan("variableOne") },
        { variableNameTwo, std::nan("variableTwo") },
    };
    min = std::nan("-inifinity");
    max = std::nan("inifinity");
    separation = std::nan("inifinity");
}

double SeparationTracker::calculateSeparation() {
    // If at least 1 of the variables being tracked is NaN, set current
    // separation to be NaN and return NaN.
    for (auto it = variablesTracker.begin(); it != variablesTracker.end(), ++it) {
        if (std::isnan(it->second)) {
            separation = std::nan("inifinity");
            return separation;
        }
    }

    // If all variables tracked are valid, calculate the separation.

}
