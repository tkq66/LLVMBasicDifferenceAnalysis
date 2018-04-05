#ifndef VALTRK_H
#define VALTRK_H

#include <string>
#include <unordered_map>
#include <functional>
#include "llvm/IR/Instruction.h"

class ValueTracker {
    private:
        std::unordered_map<std::string, double> variablesTracker;

        void allocateNewVariable(AllocaInst* i);
        void storeValueIntoVariable(StoreInst* i);
        void loadVariableIntoRegister(LoadInst* i);
        void processCalculation(BinaryOperator* i);
        void calculateArithmetic(BinaryOperator* i, std::function<double(double, double)> callback);
        double addCallback(double accumulator, double current);
        double subCallback(double accumulator, double current);
        double mulCallback(double accumulator, double current);
        double sremCallback(double accumulator, double current);
    public:
        void printTracker();
        void processNewEntry(Instruction *i);
};

#endif
