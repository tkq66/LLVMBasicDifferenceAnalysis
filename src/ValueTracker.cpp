#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Constants.h"
#include "ValueTracker.h"

void ValueTracker::printTracker() {
    for (auto variable = variablesTracker.begin(); variable != variablesTracker.end(); ++variable) {
        printf("Key: %s - Value: %lf\n", variable->first.c_str(), variable->second);
    }
    printf("\n");
}

double ValueTracker::selectVariable(std::string name) {
    return (variablesTracker.find(name) != variablesTracker.end()) ? variablesTracker[name] : std::nan("inifinity");
}

void ValueTracker::editVariable(std::string name, double value) {
    variablesTracker[name] = value;
}

void ValueTracker::processNewEntry(Instruction* i) {
    if (isa<AllocaInst>(i)) {
        allocateNewVariable(dyn_cast<AllocaInst>(i));
    }
    else if (isa<StoreInst>(i)) {
        storeValueIntoVariable(dyn_cast<StoreInst>(i));
    }
    else if (isa<LoadInst>(i)) {
        loadVariableIntoRegister(dyn_cast<LoadInst>(i));
    }
    else if (isa<BinaryOperator>(i)) {
        processCalculation(dyn_cast<BinaryOperator>(i));
    }
}

void ValueTracker::allocateNewVariable(AllocaInst* i) {
    std::string varName;
    if (!i->hasName()) {
        return;
    }
    varName = i->getName().str();
    std::pair<std::string, double> variable = std::make_pair(varName, std::nan("inifinity"));
    variablesTracker.insert(variable);
}

void ValueTracker::storeValueIntoVariable(StoreInst* i) {
    double src;
    if (i->getOperand(0)->hasName()) {
        std::unordered_map<std::string, double>::const_iterator existingVariable = variablesTracker.find(i->getOperand(0)->getName().str());
        src = existingVariable->second;
    }
    else {
        ConstantInt* ci = dyn_cast<ConstantInt>(i->getOperand(0));
        src = ci->getSExtValue();
    }
    std::string dest = i->getOperand(1)->getName().str();
    variablesTracker[dest] = src;
}

void ValueTracker::loadVariableIntoRegister(LoadInst* i) {
    std::string variableName = i->getOperand(0)->getName().str();
    double variableValue = variablesTracker[variableName];
    std::stringstream registerValue;
    registerValue << (void*)i;
    std::string registerString = registerValue.str();
    std::pair<std::string, double> registerVariable = std::make_pair(registerString, variableValue);
    variablesTracker.insert(registerVariable);
}

void ValueTracker::processCalculation(BinaryOperator* i) {
    std::function<double(double, double)> calculation;
    switch (i->getOpcode()) {
        case Instruction::Add:
            calculation = std::bind(&ValueTracker::addCallback, this, std::placeholders::_1, std::placeholders::_2);
            break;
        case Instruction::Mul:
            calculation = std::bind(&ValueTracker::mulCallback, this, std::placeholders::_1, std::placeholders::_2);
            break;
        case Instruction::Sub:
            calculation = std::bind(&ValueTracker::subCallback, this, std::placeholders::_1, std::placeholders::_2);
            break;
        case Instruction::SRem:
            calculation = std::bind(&ValueTracker::sremCallback, this, std::placeholders::_1, std::placeholders::_2);
            break;
        default:
            break;
    }
    calculateArithmetic(i, calculation);
}

void ValueTracker::calculateArithmetic(BinaryOperator* i, std::function<double(double, double)> callback) {
    double destValue;
    for (auto val = i->value_op_begin(); val != i->value_op_end(); ++val) {
        std::string currentName;
        double currentValue;
        if (val->hasName()) {
            currentName = val->getName().str();
            currentValue = variablesTracker[currentName];
        }
        else if (ConstantInt* numConstant = dyn_cast<ConstantInt>(*val)) {
            currentValue = numConstant->getZExtValue();
        }
        else {
            std::stringstream registerValue;
            registerValue << *val;
            currentName = registerValue.str();
            currentValue = variablesTracker[currentName];
        }
        destValue = (val == i->value_op_begin()) ? currentValue : callback(destValue, currentValue);
    }
    std::string destName = i->getName().str();
    std::pair<std::string, double> calculatedVariable = std::make_pair(destName, destValue);
    variablesTracker.insert(calculatedVariable);
}

double ValueTracker::addCallback(double accumulator, double current) {
    if (std::isnan(accumulator) ||
        std::isnan(current)) {
        return std::nan("inifinity");
    }
    else {
        return accumulator + current;
    }
}

double ValueTracker::subCallback(double accumulator, double current) {
    if (std::isnan(accumulator) ||
        std::isnan(current)) {
        return std::nan("inifinity");
    }
    else {
        return accumulator - current;
    }
}

double ValueTracker::mulCallback(double accumulator, double current) {
    if (std::isnan(accumulator) ||
        std::isnan(current)) {
        return std::nan("inifinity");
    }
    else {
        return accumulator * current;
    }
}

double ValueTracker::sremCallback(double accumulator, double current) {
    if (std::isnan(current)) {
        return std::nan("inifinity");
    }
    else {
        return current;
    }
}
