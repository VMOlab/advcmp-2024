// Advanced Compilers
// VMO Lab.
#ifndef INSTRUCTIONCOUNTER_H
#define INSTRUCTIONCOUNTER_H

#include "llvm/ADT/StringMap.h"
#include "llvm/IR/PassManager.h"

class InstructionCounter : public llvm::AnalysisInfoMixin<InstructionCounter> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM);
  static bool isRequired() { return false; }

private:
  static llvm::AnalysisKey Key;
  llvm::StringMap<int> InstructionCounter;
};

#endif // INSTRUCTIONCOUNTER_H