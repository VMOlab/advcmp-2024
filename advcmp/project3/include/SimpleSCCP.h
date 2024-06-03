// Advanced Compilers
// VMO Lab.
#ifndef SIMPLESCCP_H
#define SIMPLESCCP_H

#include "SparseDataflowFramework.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>
#include <cstdint>

/**
 * Lattice value for constant propgation.
 *
 * This class defines the lattice for integer constants.
 * Assume that all the integers in the prgoram (IR) can fit in 64-bit int.
 * The lattice consists of three types of its values;
 * Top: A variable is `undefined`.
 * Integer Constant: A variable is constant and has that value.
 * Bottom: A variable is not a constant.
 *
 * In this pass, all variables will start from the `Top` and move downwards
 * via `meet`.
 *
 *                                TOP
 *              /       /          |          \      \
 *     ...    -2       -1          0          1       2      ...
 *              \       \          |          /      /
 *                                BOT
 */
class ConstantValue : public LatticeValue<ConstantValue> {
public:
  ConstantValue(bool IsTop = true, bool IsBot = false)
      : IsTop(IsTop), IsBot(IsBot) {
    assert(IsTop ^ IsBot && "Can't be both Top and Bottom at same time.");
  }
  ConstantValue(int64_t N) : IsTop(false), IsBot(false), IntValue(N) {}
  ConstantValue(int N) : ConstantValue(static_cast<int64_t>(N)) {}
  ConstantValue(const ConstantValue &Other)
      : IsTop(Other.IsTop), IsBot(Other.IsBot), IntValue(Other.IntValue) {}
  ~ConstantValue() = default;

  ConstantValue &operator=(const ConstantValue &Other) = default;
  bool operator==(const ConstantValue &Other) const;
  bool operator!=(const ConstantValue &Other) const;

  static ConstantValue top() { return ConstantValue(true, false); }
  static ConstantValue bot() { return ConstantValue(false, true); }

  ConstantValue meet(const ConstantValue &Other) const;

  bool isTop() const { return IsTop && !IsBot; }
  bool isBot() const { return !IsTop && IsBot; }
  bool isConstant() const { return !IsTop && !IsBot; }
  int64_t value() const { return IntValue; }

private:
  bool IsTop; // Undefined.
  bool IsBot; // Not a constant.
  int64_t IntValue;
};

/**
 * Wrapper class for CFG edge.
 */
typedef struct CFGEdge {
  const llvm::BasicBlock *From;
  const llvm::BasicBlock *To;

  bool operator<(const struct CFGEdge &Other) const;
  bool operator==(const struct CFGEdge &Other) const;

  friend llvm::raw_ostream &operator<<(llvm::raw_ostream &ROS,
                                       const CFGEdge &CE);
} CFGEdge;

/**
 * Simple Sparse Conditional Constant Propgation Pass for integers.
 */
class SimpleSCCPAnalysis : public SparseDataflowFramework<ConstantValue> {
public:
  using DataflowFactsTy = llvm::DenseMap<llvm::Value *, ConstantValue>;
  using Result = DataflowFactsTy;

private:
  struct InstructionVisitor
      : public llvm::InstVisitor<InstructionVisitor, ConstantValue> {
    InstructionVisitor(SimpleSCCPAnalysis &Pass) : ThePass(Pass) {}

    ConstantValue visitPHINode(const llvm::PHINode &I);
    ConstantValue visitBranchInst(const llvm::BranchInst &I);
    ConstantValue visitICmpInst(const llvm::ICmpInst &I);
    ConstantValue visitBinaryOperator(const llvm::BinaryOperator &I);

    ConstantValue visitInstruction(const llvm::Instruction &I);

    // An anchor to access the class surrounding the visitor.
    SimpleSCCPAnalysis &ThePass;
  };

  static const int DefaultSize = 16;

public:
  SimpleSCCPAnalysis() : TheVisitor(*this) {}
  SimpleSCCPAnalysis(const SimpleSCCPAnalysis &Other) = delete;
  SimpleSCCPAnalysis(SimpleSCCPAnalysis &&Other)
      : DataflowFacts(std::move(Other.DataflowFacts)),
        CFGWorkset(std::move(Other.CFGWorkset)),
        SSAWorkset(std::move(Other.SSAWorkset)),
        ExecutableEdges(std::move(Other.ExecutableEdges)), TheVisitor(*this) {}

  SimpleSCCPAnalysis &operator=(const SimpleSCCPAnalysis &Other) = delete;
  SimpleSCCPAnalysis &operator=(SimpleSCCPAnalysis &&Other) = delete;

  // Pass' function-wise main.
  DataflowFactsTy run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);

private:
  // Special Key(ID) used to identify the pass.
  static llvm::AnalysisKey Key;

  // SCCP algorithm.
  void analyze(llvm::Function &F);
  void visit(const llvm::Instruction &I);

  // Helpers.
  bool isFirstVisit(const llvm::BasicBlock &BB);
  bool isExecutableBlock(const llvm::BasicBlock &BB);
  bool isExecutableEdge(const CFGEdge &CE);

  void appendExecutableSuccessors(const llvm::BranchInst &I);
  ConstantValue getConstantValue(const llvm::Value &Value);

  DataflowFactsTy DataflowFacts;
  llvm::SmallSet<CFGEdge, DefaultSize> CFGWorkset;
  llvm::SmallPtrSet<const llvm::Instruction *, DefaultSize> SSAWorkset;
  llvm::SmallSet<CFGEdge, DefaultSize> ExecutableEdges;
  InstructionVisitor TheVisitor;
};

class SimpleSCCPTransform : public llvm::PassInfoMixin<SimpleSCCPTransform> {
public:
  using AnalysisResultTy = SimpleSCCPAnalysis::Result;

  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM);
  static bool isRequired() { return false; }

private:
  bool foldConstants(llvm::Function &F, AnalysisResultTy &DataflowFacts);
};

/**
 * Printer pass which prints the analysis result from `SimpleSCCPAnalysis`.
 *
 * If you wish to print the result of the analysis to the stream, you may call
 * this printer pass at opt via `-passes=print<simple-sccp>`.
 */
class SimpleSCCPPrinter : public llvm::PassInfoMixin<SimpleSCCPPrinter> {
public:
  explicit SimpleSCCPPrinter(llvm::raw_ostream &ROS) : ROS(ROS) {}
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM);
  static bool isRequired() { return false; }

private:
  llvm::raw_ostream &ROS;
};

llvm::raw_ostream &operator<<(llvm::raw_ostream &ROS, const ConstantValue &C);
llvm::raw_ostream &operator<<(llvm::raw_ostream &ROS,
                              const SimpleSCCPAnalysis::DataflowFactsTy &DF);

#endif // SIMPLESCCP_H