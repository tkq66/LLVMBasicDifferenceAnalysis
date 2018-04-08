/**
 *  DifferencePass.cpp
 *
 *  Performs difference analysis on simple programs over an LLVM IR pass.
 *
 *  Created by CS5218 - Principles of Program Analysis
 *  Modified by Teekayu Klongtruajrok (A0174348X)
 *  Contact: e0210381@u.nus.edu
 */
#include <cstdio>
#include <iostream>
#include <set>
#include <stack>
#include <sstream>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Constants.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "SeparationTracker.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define MAIN_FUNCTION "main"
#define LOOP_BEGIN_BLOCK_NAME "while.cond"
#define LOOP_END_BLOCK_NAME "while.end"

using namespace llvm;

enum AnalyzeLoopBackedgeSwtch {
    ON,
    OFF
};

void generateCFG (BasicBlock*, SeparationTracker* separationTracker, std::stack<BasicBlock*>, AnalyzeLoopBackedgeSwtch);
void analyzeDifference (BasicBlock*, SeparationTracker* separationTracker);
bool isSameBlock (BasicBlock*, BasicBlock*);
bool isMainFunction (const char*);
bool isBeginLoop (const char*);
bool isEndLoop (const char*);
void printVars (std::set<Instruction*>);
void printInsts(std::set<Instruction*>);
void printLLVMValue (Value* v);

int main (int argc, char **argv) {
    // Read the IR file.
    LLVMContext Context;
    SMDiagnostic Err;
    std::unique_ptr<Module> M = parseIRFile(argv[1], Err, Context);
    if (M == nullptr) {
        fprintf(stderr, "error: failed to load LLVM IR file \"%s\"", argv[1]);
        return EXIT_FAILURE;
    }

    SeparationTracker* separationTracker = new SeparationTracker(argv[2], argv[3]);

    for (auto &F: *M) {
        if (isMainFunction(F.getName().str().c_str())) {
            BasicBlock* BB = dyn_cast<BasicBlock>(F.begin());
            std::stack<BasicBlock*> loopCallStack;
            generateCFG(BB, separationTracker, loopCallStack, ON);
        }
    }

    return 0;
}


void generateCFG (BasicBlock* BB, SeparationTracker* separationTracker, std::stack<BasicBlock*> loopCallStack, AnalyzeLoopBackedgeSwtch backedgeSwitch) {
  const char *blockName = BB->getName().str().c_str();
  printf("Label Name:%s\n", blockName);

  // Create local copies of parameters that can be updated
  AnalyzeLoopBackedgeSwtch newBackedgeSwitch = backedgeSwitch;
  std::stack<BasicBlock*> newLoopCallStack = !loopCallStack.empty() ? std::stack<BasicBlock*>(loopCallStack) : std::stack<BasicBlock*>();

  // Track loop layer by pushing them into the stack
  if (isBeginLoop(blockName)) {
      newLoopCallStack.push(BB);
  }
  // Untrack the loop when a loop ends
  if (isEndLoop(blockName)) {
      newLoopCallStack.pop();
      // Turn back on to prepare for any outer loops
      newBackedgeSwitch = ON;
  }

  analyzeDifference(BB, separationTracker);

  // Pass secretVars list to child BBs and check them
  const TerminatorInst *tInst = BB->getTerminator();
  int branchCount = tInst->getNumSuccessors();

  for (int i = 0;  i < branchCount; ++i) {
      BasicBlock *next = tInst->getSuccessor(i);
      BasicBlock *prevLoopBegin = !newLoopCallStack.empty() ? newLoopCallStack.top() : nullptr;

      // If still analyzing loop backedge and the loop is going past the calling point, stop this recursion
      if (isEndLoop(next->getName().str().c_str()) &&
          (newBackedgeSwitch == OFF)) {
          return;
      }
      // Analyze loop backedge by running through the loop one more time before ending
      // to complete taint analysis of variable dependencies
      if (isEndLoop(next->getName().str().c_str()) &&
          (newBackedgeSwitch == ON) &&
          !newLoopCallStack.empty()) {
          // prevent repeating backedge analysis loop
          newBackedgeSwitch = OFF;
          generateCFG(next, separationTracker, newLoopCallStack, newBackedgeSwitch);
      }
      // Terminate looping condition to acheive least fixed point solution
      if (isSameBlock(prevLoopBegin, next)) {
          return;
      }
      // Analyze the next instruction and get all the discovered from that analysis context
      generateCFG(next, separationTracker, newLoopCallStack, newBackedgeSwitch);
  }

  separationTracker->printSeparationReport();

  return;
}

void analyzeDifference (BasicBlock* BB, SeparationTracker* separationTracker) {
    // Loop through instructions in BB
    double separation;
    for (auto &I: *BB) {
        separation = separationTracker->processNewEntry(&I);
        separationTracker->printSeparationReport();
    }
    return;
}

bool isSameBlock (BasicBlock* blockA, BasicBlock* blockB) {
    // Prevent nullptr reference
    if (!blockA || !blockB) {
        return false;
    }
    return blockA->getName().str() == blockB->getName().str();
}

bool isMainFunction (const char* functionName) {
    return strncmp(functionName, MAIN_FUNCTION, strlen(MAIN_FUNCTION)) == 0;
}

bool isBeginLoop (const char* instructionName) {
    return strncmp(instructionName, LOOP_BEGIN_BLOCK_NAME, strlen(LOOP_BEGIN_BLOCK_NAME)) == 0;
}

bool isEndLoop (const char* instructionName) {
    return strncmp(instructionName, LOOP_END_BLOCK_NAME, strlen(LOOP_END_BLOCK_NAME)) == 0;
}

void printVars (std::set<Instruction*> vars) {
    for (auto &S: vars) {
        printf("%s ", S->getName().str().c_str() );
    }
    printf("\n");
}

void printInsts(std::set<Instruction*> insts) {
    for (auto &S: insts) {
        printLLVMValue(dyn_cast<Value>(S));
    }
    printf("\n");
}

void printLLVMValue (Value* v) {
    std::string res;
    raw_string_ostream rso(res);
    v->print(rso, true);
    printf("%s\n", res.c_str());
}
