//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Executor.cc
//---------------------------------------------------------------------------//
#include "Executor.hh"

#include <iostream>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>

#include "Assert.hh"
#include "Module.hh"
#include "QuantumInterface.hh"
#include "RuntimeInterface.hh"
#include "detail/EndGuard.hh"
#include "detail/GlobalMapper.hh"

namespace qiree
{
namespace
{
//---------------------------------------------------------------------------//
/*!
 * Pointer to active interfaces.
 *
 * LLVM's addGlobalMapping requires a global function symbol rather than a
 * std::function.
 */
static QuantumInterface* q_interface_{nullptr};
static RuntimeInterface* r_interface_{nullptr};

//---------------------------------------------------------------------------//
//! Generate a function name without a specialization suffix
#define QIREE_RT_FUNCTION(FUNC) quantum__rt__##FUNC

//! Generate a function name with a specialization suffix
#define QIREE_QIS_FUNCTION(FUNC, SUFFIX) quantum__qis__##FUNC##__##SUFFIX

//---------------------------------------------------------------------------//
//!@{
/*!
 * QIR function wrappers.
 *
 * \note These are generated from scripts/dev/generate-bindings.py .
 */
//---------------------------------------------------------------------------//
// MEASUREMENTS
//---------------------------------------------------------------------------//
std::uintptr_t QIREE_QIS_FUNCTION(m, body)(std::uintptr_t arg1)
{
    return q_interface_->m(Qubit{arg1}).value;
}
std::uintptr_t
QIREE_QIS_FUNCTION(measure, body)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->measure(Array{arg1}, Array{arg2}).value;
}
std::uintptr_t QIREE_QIS_FUNCTION(mresetz, body)(std::uintptr_t arg1)
{
    return q_interface_->mresetz(Qubit{arg1}).value;
}
void QIREE_QIS_FUNCTION(mz, body)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->mz(Qubit{arg1}, Result{arg2});
}
bool QIREE_QIS_FUNCTION(read_result, body)(std::uintptr_t arg1)
{
    return static_cast<bool>(q_interface_->read_result(Result{arg1}));
}
//---------------------------------------------------------------------------//
// GATES
//---------------------------------------------------------------------------//
void QIREE_QIS_FUNCTION(ccx, body)(std::uintptr_t arg1,
                                   std::uintptr_t arg2,
                                   std::uintptr_t arg3)
{
    return q_interface_->ccx(Qubit{arg1}, Qubit{arg2}, Qubit{arg3});
}
void QIREE_QIS_FUNCTION(cnot, body)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->cnot(Qubit{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(cx, body)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->cx(Qubit{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(cy, body)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->cy(Qubit{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(cz, body)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->cz(Qubit{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(exp, adj)(std::uintptr_t arg1,
                                  double arg2,
                                  std::uintptr_t arg3)
{
    return q_interface_->exp_adj(Array{arg1}, arg2, Array{arg3});
}
void QIREE_QIS_FUNCTION(exp, body)(std::uintptr_t arg1,
                                   double arg2,
                                   std::uintptr_t arg3)
{
    return q_interface_->exp(Array{arg1}, arg2, Array{arg3});
}
void QIREE_QIS_FUNCTION(exp, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->exp(Array{arg1}, Tuple{arg2});
}
void QIREE_QIS_FUNCTION(exp, ctladj)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->exp_adj(Array{arg1}, Tuple{arg2});
}
void QIREE_QIS_FUNCTION(h, body)(std::uintptr_t arg1)
{
    return q_interface_->h(Qubit{arg1});
}
void QIREE_QIS_FUNCTION(h, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->h(Array{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(r,
                        adj)(pauli_type arg1, double arg2, std::uintptr_t arg3)
{
    return q_interface_->r_adj(static_cast<Pauli>(arg1), arg2, Qubit{arg3});
}
void QIREE_QIS_FUNCTION(r,
                        body)(pauli_type arg1, double arg2, std::uintptr_t arg3)
{
    return q_interface_->r(static_cast<Pauli>(arg1), arg2, Qubit{arg3});
}
void QIREE_QIS_FUNCTION(r, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->r(Array{arg1}, Tuple{arg2});
}
void QIREE_QIS_FUNCTION(r, ctladj)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->r_adj(Array{arg1}, Tuple{arg2});
}
void QIREE_QIS_FUNCTION(reset, body)(std::uintptr_t arg1)
{
    return q_interface_->reset(Qubit{arg1});
}
void QIREE_QIS_FUNCTION(rx, body)(double arg1, std::uintptr_t arg2)
{
    return q_interface_->rx(arg1, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(rx, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->rx(Array{arg1}, Tuple{arg2});
}
void QIREE_QIS_FUNCTION(rxx, body)(double arg1,
                                   std::uintptr_t arg2,
                                   std::uintptr_t arg3)
{
    return q_interface_->rxx(arg1, Qubit{arg2}, Qubit{arg3});
}
void QIREE_QIS_FUNCTION(ry, body)(double arg1, std::uintptr_t arg2)
{
    return q_interface_->ry(arg1, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(ry, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->ry(Array{arg1}, Tuple{arg2});
}
void QIREE_QIS_FUNCTION(ryy, body)(double arg1,
                                   std::uintptr_t arg2,
                                   std::uintptr_t arg3)
{
    return q_interface_->ryy(arg1, Qubit{arg2}, Qubit{arg3});
}
void QIREE_QIS_FUNCTION(rz, body)(double arg1, std::uintptr_t arg2)
{
    return q_interface_->rz(arg1, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(rz, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->rz(Array{arg1}, Tuple{arg2});
}
void QIREE_QIS_FUNCTION(rzz, body)(double arg1,
                                   std::uintptr_t arg2,
                                   std::uintptr_t arg3)
{
    return q_interface_->rzz(arg1, Qubit{arg2}, Qubit{arg3});
}
void QIREE_QIS_FUNCTION(s, adj)(std::uintptr_t arg1)
{
    return q_interface_->s_adj(Qubit{arg1});
}
void QIREE_QIS_FUNCTION(s, body)(std::uintptr_t arg1)
{
    return q_interface_->s(Qubit{arg1});
}
void QIREE_QIS_FUNCTION(s, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->s(Array{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(s, ctladj)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->s_adj(Array{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(swap, body)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->swap(Qubit{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(t, adj)(std::uintptr_t arg1)
{
    return q_interface_->t_adj(Qubit{arg1});
}
void QIREE_QIS_FUNCTION(t, body)(std::uintptr_t arg1)
{
    return q_interface_->t(Qubit{arg1});
}
void QIREE_QIS_FUNCTION(t, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->t(Array{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(t, ctladj)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->t_adj(Array{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(x, body)(std::uintptr_t arg1)
{
    return q_interface_->x(Qubit{arg1});
}
void QIREE_QIS_FUNCTION(x, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->x(Array{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(y, body)(std::uintptr_t arg1)
{
    return q_interface_->y(Qubit{arg1});
}
void QIREE_QIS_FUNCTION(y, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->y(Array{arg1}, Qubit{arg2});
}
void QIREE_QIS_FUNCTION(z, body)(std::uintptr_t arg1)
{
    return q_interface_->z(Qubit{arg1});
}
void QIREE_QIS_FUNCTION(z, ctl)(std::uintptr_t arg1, std::uintptr_t arg2)
{
    return q_interface_->z(Array{arg1}, Qubit{arg2});
}
//---------------------------------------------------------------------------//
// ASSERTIONS
//---------------------------------------------------------------------------//
void QIREE_QIS_FUNCTION(assertmeasurementprobability,
                        body)(std::uintptr_t arg1,
                              std::uintptr_t arg2,
                              std::uintptr_t arg3,
                              double arg4,
                              std::uintptr_t arg5,
                              double arg6)
{
    return q_interface_->assertmeasurementprobability(
        Array{arg1}, Array{arg2}, Result{arg3}, arg4, String{arg5}, arg6);
}
void QIREE_QIS_FUNCTION(assertmeasurementprobability, ctl)(std::uintptr_t arg1,
                                                           std::uintptr_t arg2)
{
    return q_interface_->assertmeasurementprobability(Array{arg1}, Tuple{arg2});
}

//---------------------------------------------------------------------------//
// RUNTIME
//---------------------------------------------------------------------------//
void QIREE_RT_FUNCTION(initialize)(OptionalCString env)
{
    return r_interface_->initialize(env);
}
void QIREE_RT_FUNCTION(array_record_output)(size_type s, OptionalCString tag)
{
    return r_interface_->array_record_output(s, tag);
}
void QIREE_RT_FUNCTION(tuple_record_output)(size_type s, OptionalCString tag)
{
    return r_interface_->tuple_record_output(s, tag);
}
void QIREE_RT_FUNCTION(result_record_output)(std::uintptr_t r,
                                             OptionalCString tag)
{
    return r_interface_->result_record_output(Result{r}, tag);
}

//!@}
//---------------------------------------------------------------------------//
}  // namespace

//---------------------------------------------------------------------------//
/*!
 * Construct with a QIR input filename.
 */
Executor::Executor(Module&& module)
    : entrypoint_{module.entrypoint_}, module_{module.module_.get()}
{
    QIREE_EXPECT(module);
    QIREE_EXPECT(entrypoint_ && module_);

    // Save module and entry point attributes
    entry_point_attrs_ = module.load_entry_point_attrs();
    module_flags_ = module.load_module_flags();

    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    LLVMLinkInMCJIT();

    // Create execution engine by capturing the module
    ee_ = [&module] {
        llvm::EngineBuilder builder{std::move(module.module_)};

        // Pass a reference to a string for diagnosing errors
        std::string err_str;
        builder.setErrorStr(&err_str);

        // Set execution options
        llvm::TargetOptions opts;
        opts.ExceptionModel = llvm::ExceptionHandling::DwarfCFI;
        builder.setTargetOptions(opts);

        // Create the builder, or throw an exception with the failure
        std::unique_ptr<llvm::ExecutionEngine> ee{builder.create()};
        QIREE_VALIDATE(ee, << "failed to create execution engine: " << err_str);
        return ee;
    }();

    // Suppress symbol lookup in system dynamic libraries
    ee_->DisableSymbolSearching(true);

    // Add "lazy function creator" that just gives a more informative message
    ee_->InstallLazyFunctionCreator([](std::string const& s) -> void* {
        QIREE_NOT_IMPLEMENTED(s.c_str());
    });

    // Bind functions if available
    detail::GlobalMapper bind_function(*module_, ee_.get());
#define QIREE_BIND_RT_FUNCTION(FUNC) \
    bind_function("__quantum__rt__" #FUNC, QIREE_RT_FUNCTION(FUNC))
#define QIREE_BIND_QIS_FUNCTION(FUNC, SUFFIX)            \
    bind_function("__quantum__qis__" #FUNC "__" #SUFFIX, \
                  QIREE_QIS_FUNCTION(FUNC, SUFFIX))
    // Measurements
    QIREE_BIND_QIS_FUNCTION(m, body);
    QIREE_BIND_QIS_FUNCTION(measure, body);
    QIREE_BIND_QIS_FUNCTION(mresetz, body);
    QIREE_BIND_QIS_FUNCTION(mz, body);
    QIREE_BIND_QIS_FUNCTION(read_result, body);
    // Gates
    QIREE_BIND_QIS_FUNCTION(ccx, body);
    QIREE_BIND_QIS_FUNCTION(cnot, body);
    QIREE_BIND_QIS_FUNCTION(cx, body);
    QIREE_BIND_QIS_FUNCTION(cy, body);
    QIREE_BIND_QIS_FUNCTION(cz, body);
    QIREE_BIND_QIS_FUNCTION(exp, adj);
    QIREE_BIND_QIS_FUNCTION(exp, body);
    QIREE_BIND_QIS_FUNCTION(exp, ctl);
    QIREE_BIND_QIS_FUNCTION(exp, ctladj);
    QIREE_BIND_QIS_FUNCTION(h, body);
    QIREE_BIND_QIS_FUNCTION(h, ctl);
    QIREE_BIND_QIS_FUNCTION(r, adj);
    QIREE_BIND_QIS_FUNCTION(r, body);
    QIREE_BIND_QIS_FUNCTION(r, ctl);
    QIREE_BIND_QIS_FUNCTION(r, ctladj);
    QIREE_BIND_QIS_FUNCTION(reset, body);
    QIREE_BIND_QIS_FUNCTION(rx, body);
    QIREE_BIND_QIS_FUNCTION(rx, ctl);
    QIREE_BIND_QIS_FUNCTION(rxx, body);
    QIREE_BIND_QIS_FUNCTION(ry, body);
    QIREE_BIND_QIS_FUNCTION(ry, ctl);
    QIREE_BIND_QIS_FUNCTION(ryy, body);
    QIREE_BIND_QIS_FUNCTION(rz, body);
    QIREE_BIND_QIS_FUNCTION(rz, ctl);
    QIREE_BIND_QIS_FUNCTION(rzz, body);
    QIREE_BIND_QIS_FUNCTION(s, adj);
    QIREE_BIND_QIS_FUNCTION(s, body);
    QIREE_BIND_QIS_FUNCTION(s, ctl);
    QIREE_BIND_QIS_FUNCTION(s, ctladj);
    QIREE_BIND_QIS_FUNCTION(swap, body);
    QIREE_BIND_QIS_FUNCTION(t, adj);
    QIREE_BIND_QIS_FUNCTION(t, body);
    QIREE_BIND_QIS_FUNCTION(t, ctl);
    QIREE_BIND_QIS_FUNCTION(t, ctladj);
    QIREE_BIND_QIS_FUNCTION(x, body);
    QIREE_BIND_QIS_FUNCTION(x, ctl);
    QIREE_BIND_QIS_FUNCTION(y, body);
    QIREE_BIND_QIS_FUNCTION(y, ctl);
    QIREE_BIND_QIS_FUNCTION(z, body);
    QIREE_BIND_QIS_FUNCTION(z, ctl);
    // Assertions
    QIREE_BIND_QIS_FUNCTION(assertmeasurementprobability, body);
    QIREE_BIND_QIS_FUNCTION(assertmeasurementprobability, ctl);

    QIREE_BIND_RT_FUNCTION(initialize);
    QIREE_BIND_RT_FUNCTION(array_record_output);
    QIREE_BIND_RT_FUNCTION(tuple_record_output);
    QIREE_BIND_RT_FUNCTION(result_record_output);
#undef QIREE_BIND_RT_FUNCTION
#undef QIREE_BIND_QIS_FUNCTION

    QIREE_ENSURE(!module);
}

//---------------------------------------------------------------------------//
//! Default destructor
Executor::~Executor() = default;

//---------------------------------------------------------------------------//
/*!
 * Execute with the given interface functions.
 */
void Executor::operator()(QuantumInterface& qi, RuntimeInterface& ri) const
{
    QIREE_EXPECT(ee_);

    QIREE_VALIDATE(!q_interface_ && !r_interface_,
                   << "cannot call LLVM executor recursively or in MT "
                      "environment (for now)");
    detail::EndGuard on_end_scope_([] {
        q_interface_->tear_down();
        q_interface_ = nullptr;
        r_interface_ = nullptr;
    });
    q_interface_ = &qi;
    r_interface_ = &ri;

    // Call setup on the interface
    qi.set_up(entry_point_attrs_);

    // Execute the main function
    auto result = ee_->runFunction(entrypoint_, {});
    QIREE_DISCARD(result);
}

//---------------------------------------------------------------------------//
}  // namespace qiree
