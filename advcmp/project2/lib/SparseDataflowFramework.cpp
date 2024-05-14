// Advanced Compilers
// VMO Lab.
#include "SparseDataflowFramework.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

std::string getId(const llvm::Value &V) {
  std::string String;
  raw_string_ostream OS(String);

  V.printAsOperand(OS, false);
  return OS.str();
}

std::string getId(const llvm::Value *V) {
  return getId(*V);
}