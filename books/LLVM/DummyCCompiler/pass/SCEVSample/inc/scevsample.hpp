#ifndef _SCEV_SAMPLE_H__
#define _SCEV_SAMPLE_H__

#include<cstdio>
#include"llvm/Analysis/LoopPass.h"
#include"llvm/Analysis/LoopInfo.h"
#include"llvm/Analysis/ScalarEvolution.h"
#include"llvm/IR/Constants.h"
#include"llvm/IR/Instructions.h"
#include"llvm/IR/Operator.h"
#include"llvm/IR/Function.h"
#include"llvm/Pass.h"
#include"llvm/Support/Casting.h"

class SCEVSample : public llvm::LoopPass{
	public:
		static char ID;
		SCEVSample() : llvm::LoopPass(ID){}
		~SCEVSample(){}

		virtual bool runOnLoop(llvm::Loop *L, llvm::LPPassManager &LPM);
		virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
		llvm::APInt getIterateTime();
};

#endif
