#include <string>
#include <tuple>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Constants.h"
#include "SeparationTracker.h"
#include "ValueTracker.h"

SeparationTracker::SeparationTracker(std::string varNameOne, std::string varNameTwo) {
    variableNames = std::make_tuple(varNameOne, varNameTwo);
    separation = std::nan("inifinity");
}

double SeparationTracker::processNewEntry(Instruction* i) {
    valueTracker.processNewEntry(i);
    return calculateSeparation();
}

void SeparationTracker::printSeparationReport() {
    const char * varNameOne = std::get<0>(variableNames).c_str();
    const char * varNameTwo = std::get<1>(variableNames).c_str();
    double varOne = valueTracker.selectVariable(varNameOne);
    double varTwo = valueTracker.selectVariable(varNameTwo);
    printf("Sep between '%s' - %lf and '%s' - %lf = %lf", varNameOne, varOne, varNameTwo, varTwo, separation);
    printf("\n");
}

void SeparationTracker::printVariableTracker() {
    valueTracker.printTracker();
}

double SeparationTracker::calculateSeparation() {
    std::string varNameOne = std::get<0>(variableNames);
    std::string varNameTwo = std::get<1>(variableNames);
    double varOne = valueTracker.selectVariable(varNameOne);
    double varTwo = valueTracker.selectVariable(varNameTwo);
    if (std::isnan(varOne) || std::isnan(varTwo)) {
        separation = std::nan("inifinity");
    }
    else {
        separation = (varOne >= varTwo) ? (varOne - varTwo) : (varTwo - varOne);
    }
    return separation;
}

double SeparationTracker::getSeparation() {
    return separation;
}
