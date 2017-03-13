

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/GraphWriter.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include "CoverageProfilingPass.h"


using namespace llvm;
using namespace coverageprofiling;


namespace coverageprofiling
{
char CoverageProfilingPass::ID = 0;
}

static Constant* createConstantString(Module& m, StringRef str)
{
	auto& context = m.getContext();

	auto* name = ConstantDataArray::getString(context, str, true);
	auto* int8Ty = Type::getInt8Ty(context);

	auto* arrayTy = ArrayType::get(int8Ty, str.size() + 1);
	auto* asStr = new GlobalVariable(
			m, arrayTy, true, GlobalValue::PrivateLinkage, name);

	auto* zero = ConstantInt::get(Type::getInt32Ty(context), 0);
	Value* indices[] = {zero, zero};
	return ConstantExpr::getInBoundsGetElementPtr(arrayTy, asStr, indices);
}

bool
CoverageProfilingPass::runOnModule(llvm::Module &module)
{
	LLVMContext& context = module.getContext();
	auto* voidTy = Type::getVoidTy(context);
	auto* int64Ty = Type::getInt64Ty(context);
	auto* boolTy = Type::getInt1Ty(context);
	auto* stringTy = Type::getInt8PtrTy(context);
	auto countfunc = module.getOrInsertFunction("CoVeRaGePrOfIlInG_count",
		FunctionType::get(voidTy, {int64Ty}, false));
	DenseMap<BasicBlock*, bool> isDD;
	uint64_t functionID = 0;
	uint64_t blockId = 0;

	Type* fieldTys[] = {int64Ty, boolTy};
	auto* structTy = StructType::get(context, fieldTys, false);
	std::vector<Constant*> blocks;
	std::vector<Constant*> funcs;

	// false visited
	Constant* visitInit = ConstantInt::get(boolTy, 0);

	for (Function& f : module)
	{
		// not declarations
		if (f.isDeclaration()) continue;
		// only global linked
		if (GlobalValue::LinkOnceODRLinkage != f.getLinkage()) continue;

		Constant* fname = createConstantString(module, f.getName());
		funcs.push_back(fname);

		ConstantInt* funcId = ConstantInt::get(int64Ty, functionID, false);
		for (BasicBlock& bb : f)
		{
			BasicBlock* pred = bb.getUniquePredecessor();
			BasicBlock* succ = bb.getUniqueSuccessor();
			if (nullptr == pred || nullptr == succ || false == isDD[pred])
			{
				Instruction* termInst = bb.getTerminator();
				IRBuilder<> builder(termInst);

				ConstantInt* bbId = ConstantInt::get(int64Ty, blockId, false);
				builder.CreateCall(countfunc, {bbId});

				Constant* structFields[] =
				{
					funcId,
					visitInit
				};
				blocks.push_back(ConstantStruct::get(structTy, structFields));
				blockId++;
			}
			isDD[&bb] = pred && succ;
		}
		functionID++;
	}

	auto* tableTy = ArrayType::get(structTy, blocks.size());
	auto* blockTable = ConstantArray::get(tableTy, blocks);
	new GlobalVariable(module,
		tableTy, false, GlobalValue::ExternalLinkage,
		blockTable, "CoVeRaGePrOfIlInG_blockInfo");

	auto* arrTy = ArrayType::get(stringTy, funcs.size());
	auto* funcTable = ConstantArray::get(arrTy, funcs);
	new GlobalVariable(module,
		arrTy, false, GlobalValue::ExternalLinkage,
		funcTable, "CoVeRaGePrOfIlInG_funcStr");

	auto* numBB = ConstantInt::get(int64Ty, blocks.size(), false);
	new GlobalVariable(module,
		int64Ty, true, GlobalValue::ExternalLinkage,
		numBB, "CoVeRaGePrOfIlInG_nblocks");

	auto* numF = ConstantInt::get(int64Ty, funcs.size(), false);
	new GlobalVariable(module,
		int64Ty, true, GlobalValue::ExternalLinkage,
		numF, "CoVeRaGePrOfIlInG_nfuncs");

	auto* printer = module.getOrInsertFunction("CoVeRaGePrOfIlInG_start", voidTy, nullptr);
	appendToGlobalCtors(module, static_cast<Function*>(printer), 0);

//	auto* printer = module.getOrInsertFunction("CoVeRaGePrOfIlInG_print", voidTy, nullptr);
//	appendToGlobalDtors(module, static_cast<Function*>(printer), 0);

	return true;
}
