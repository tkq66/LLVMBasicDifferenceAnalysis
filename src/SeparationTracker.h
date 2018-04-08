#ifndef SEPTRK_H
#define SEPTRK_H

#include <string>
#include <tuple>
#include "ValueTracker.h"
#include "llvm/IR/Instruction.h"

using namespace llvm;

class SeparationTracker {
    private:
        std::tuple<std::string, std::string> variableNames;
        ValueTracker valueTracker;
        double separation;

    public:
        SeparationTracker(std::string varNameOne, std::string varNameTwo);
        double processNewEntry(Instruction *i);
        void printSeparationReport();
        void printVariableTracker();
        double calculateSeparation();
        double getSeparation();
};

#endif
