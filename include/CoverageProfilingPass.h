
#ifndef COVERAGEPROFILINGPASS_H
#define COVERAGEPROFILINGPASS_H


#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"


namespace coverageprofiling
{


struct CoverageProfilingPass : public llvm::ModulePass
{
	static char ID;

	CoverageProfilingPass()
		: llvm::ModulePass(ID)
			{ }

	void
	getAnalysisUsage(llvm::AnalysisUsage& au) const override
	{
		au.addRequired<llvm::DominatorTreeWrapperPass>();
		au.addRequired<llvm::LoopInfoWrapperPass>();
	}

	bool runOnModule(llvm::Module& m) override;
};


}	// namespace coverageprofiling


#endif

