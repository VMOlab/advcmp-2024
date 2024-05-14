// Advanced Compilers
// VMO Lab.
#ifndef SPARSEDATAFLOWFRAMEWORK_H
#define SPARSEDATAFLOWFRAMEWORK_H

#include <string>
#include <type_traits>
#include <vector>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"

/**
 * @brief Base CRTP class for lattice value.
 * @tparam LVImpl Implementation class of the lattice.
 */
template <typename LVImpl> class LatticeValue {
public:
  LatticeValue meet(const LatticeValue &Other) {
    return static_cast<LVImpl *>(this)->meet(Other);
  }
  static LVImpl top() { return LVImpl::top(); }
};

// Static type checking via SFINAE.
template <typename ValueT, typename = typename std::enable_if<std::is_base_of<
                               LatticeValue<ValueT>, ValueT>::value>::type>
class SparseDataflowFramework;

/**
 * @brief Sparse dataflow analysis function pass.
 * @tparam ValueT Value type of sub-lattice being used.
 */
template <typename ValueT>
class SparseDataflowFramework<ValueT>
    : public llvm::AnalysisInfoMixin<SparseDataflowFramework<ValueT>> {
public:
  static bool isRequired() { return false; }
  static llvm::AnalysisKey Key;
};

template <typename ValueT>
llvm::AnalysisKey SparseDataflowFramework<ValueT>::Key;

std::string getId(const llvm::Value &V);
std::string getId(const llvm::Value *V);

#endif // SPARSEDATAFLOWFRAMEWORK_H