#include <iostream>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/TargetSelect.h>

extern "C" void external_function() {
    std::cout << "External function called!" << std::endl;
}

int main() {
    // Initialize the LLVM context and module
    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>("my_module", context);

    // Initialize native target (required for the JIT)
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    // Create and configure the function to JIT compile
    llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
    llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "my_function", module.get());

    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", func);
    llvm::IRBuilder<> builder(entry);

    // Declare the external function
    llvm::FunctionType *externalFuncType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
    llvm::Function *externalFunc = llvm::Function::Create(externalFuncType, llvm::Function::ExternalLinkage, "external_function", module.get());

    // Call the external function from our function
    builder.CreateCall(externalFunc);
    builder.CreateRetVoid();

    // Create the JIT engine
    std::string errStr;
    llvm::ExecutionEngine *engine = llvm::EngineBuilder(std::move(module))
        .setErrorStr(&errStr)
        .create();

    if (!engine) {
        std::cerr << "Failed to create ExecutionEngine: " << errStr << std::endl;
        return 1;
    }

    // Map the external function into the JIT engine
    engine->addGlobalMapping(externalFunc, reinterpret_cast<void *>(external_function));

    // Compile and run the function
    engine->finalizeObject();
    engine->runFunction(func, {});

    // Cleanup
    delete engine;

    return 0;
}
