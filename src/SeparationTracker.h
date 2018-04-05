#ifndef SEPTRK_H
#define SEPTRK_H

#include <string>
#include <unordered_map>

class SeparationTracker {
    private:
        std::unordered_map<std::string, double> variablesTracker;
        double separation;
        double min;
        double max;
    public:
        SeparationTracker(std::string varNameOne, std::string varNameTwo);
        double calculateSeparation();
        double getSeparation();
        void updateVariable(std::string name, double value);
};

#endif
