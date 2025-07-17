//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qiree/Module.cc
//---------------------------------------------------------------------------//
#include "Module.hh"

#include <memory>
#include <sstream>
#include <string_view>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>

#include "Assert.hh"

using namespace std::string_view_literals;

namespace qiree
{
namespace
{
//---------------------------------------------------------------------------//
/*!
 * Shared, thread-local LLVM context.
 *
 * This must exceed the lifetime of any IR, modules, etc.
 */
llvm::LLVMContext& context()
{
    static llvm::LLVMContext ctx;
    return ctx;
}

//---------------------------------------------------------------------------//
/*!
 * Load an LLVM module from a file.
 */
std::unique_ptr<llvm::Module> load_llvm_module(std::string const& filename)
{
    llvm::SMDiagnostic err;
    auto module = llvm::parseIRFile(filename, err, context());
    if (!module)
    {
        err.print("qiree", llvm::errs());
        QIREE_VALIDATE(module,
                       << "failed to read QIR input at '" << filename << "'");
    }
    return module;
}

//---------------------------------------------------------------------------//
/*!
 * Find a function tagged with the QIR `entry_point`.
 */
llvm::Function* find_entry_point(llvm::Module& m)
{
    for (llvm::Function& f : m)
    {
        for (auto const& attr_set : f.getAttributes())
        {
            for (auto const& attr : attr_set)
            {
                if (attr.isStringAttribute())
                {
                    auto s = attr.getKindAsString();
                    if (s == "entry_point"
                        || s == "EntryPoint")  // BAD: multiplefunction.ll
                    {
                        return &f;
                    }
                }
            }
        }
    }
    return nullptr;
}

//---------------------------------------------------------------------------//
/*!
 * Interpret a string attribute as a certain type.
 */
template<class T>
void attr_get_to(llvm::Attribute const& attr, T& dest)
{
    std::istringstream is{attr.getValueAsString().str()};
    is >> dest;
    QIREE_VALIDATE(is,
                   << "failed to parse attribute '"
                   << std::string_view(attr.getKindAsString()) << "'");
}

//---------------------------------------------------------------------------//
}  // namespace

//---------------------------------------------------------------------------//
/*!
 * Construct with an LLVM module.
 */
Module::Module(UPModule&& module) : module_{std::move(module)}
{
    QIREE_EXPECT(module_);

    // Search for entry point
    entrypoint_ = find_entry_point(*module_);
    QIREE_VALIDATE(entrypoint_,
                   << "no function with QIR 'entry_point' attribute "
                      "exists in '"
                   << module_->getSourceFileName() << "'");
}

//---------------------------------------------------------------------------//
/*!
 * Construct with an LLVM module and an entry point.
 *
 * Useful when there are multiple entry points.
 */
Module::Module(UPModule&& module, std::string const& entrypoint)
    : module_{std::move(module)}
{
    QIREE_EXPECT(module_);

    // Search for explicitly named entry point
    entrypoint_ = module_->getFunction(entrypoint);
    QIREE_VALIDATE(entrypoint_,
                   << "no entrypoint function '" << entrypoint << "' exists");
}

//---------------------------------------------------------------------------//
/*!
 * Construct with an LLVM IR file (bitcode or disassembled).
 */
Module::Module(std::string const& filename)
    : Module{load_llvm_module(filename)}
{
}

//---------------------------------------------------------------------------//
/*!
 * Construct with an LLVM IR file (bitcode or disassembled) and entry point.
 * Useful when there are multiple entry points.
 */
Module::Module(std::string const& filename, std::string const& entrypoint)
    : module_{load_llvm_module(filename)}
{
    QIREE_EXPECT(module_);

    // Search for explicitly named entry point
    entrypoint_ = module_->getFunction(entrypoint);
    QIREE_VALIDATE(entrypoint_,
                   << "no entrypoint function '" << entrypoint << "' exists");
}

//---------------------------------------------------------------------------//
/*!
 * Read a module by parsing an in-memory LLVM IR string.
 */
std::unique_ptr<Module> Module::from_bytes(std::string const& content)
{
    llvm::SMDiagnostic err;

    // Create memory buffer from the in-memory IR content
    std::unique_ptr<llvm::MemoryBuffer> buffer
        = llvm::MemoryBuffer::getMemBuffer(content, "<in-memory>", false);

    // Parse the IR using LLVM context
    auto llvm_module = llvm::parseIR(buffer->getMemBufferRef(), err, context());

    if (!llvm_module)
    {
        err.print("qiree", llvm::errs());
        QIREE_VALIDATE(llvm_module,
                       << "Failed to parse QIR from in-memory content '"
                       << content << "'");
    }

    // Construct and return Module from parsed llvm::Module
    return std::make_unique<Module>(std::move(llvm_module));
}

//! Construct in an empty state
Module::Module() = default;

// Default destructor and move
Module::~Module() = default;
Module::Module(Module&&) = default;
Module& Module::operator=(Module&&) = default;

//---------------------------------------------------------------------------//
/*!
 * Process entry point attributes.
 *
 * We allow for older PyQIR which incorrectly names attributes: see
 * https://github.com/qir-alliance/pyqir/issues/250 .
 */
EntryPointAttrs Module::load_entry_point_attrs() const
{
    QIREE_EXPECT(*this);

    EntryPointAttrs result;
    for (auto const& attr_set : entrypoint_->getAttributes())
    {
        for (auto const& attr : attr_set)
        {
            if (attr.isStringAttribute())
            {
                auto s = std::string_view(attr.getKindAsString());
                if (s == "required_num_qubits"sv
                    || s == "num_required_qubits"sv /* BAD PYQIR */)
                {
                    attr_get_to(attr, result.required_num_qubits);
                }
                else if (s == "required_num_results"sv
                         || s == "num_required_results"sv /* BAD PYQIR */)
                {
                    attr_get_to(attr, result.required_num_results);
                }
                else if (s == "output_labeling_schema"sv)
                {
                    result.output_labeling_schema = attr.getValueAsString();
                }
                else if (s == "qir_profiles"sv)
                {
                    result.qir_profiles = attr.getValueAsString();
                }
            }
        }
    }
    return result;
}

//---------------------------------------------------------------------------//
/*!
 * Translate module attributes into flags.
 */
ModuleFlags Module::load_module_flags() const
{
    QIREE_EXPECT(*this);

    auto get_constant_int = [this](llvm::StringRef s) {
        using llvm::mdconst::extract_or_null;
        auto* md = module_->getModuleFlag(s);
        return extract_or_null<llvm::ConstantInt>(md);
    };

    ModuleFlags flags;
#define QIREE_EXTRACT(ATTR)                     \
    do                                          \
    {                                           \
        if (auto* ci = get_constant_int(#ATTR)) \
        {                                       \
            flags.ATTR = ci->getZExtValue();    \
        }                                       \
    } while (0)
    QIREE_EXTRACT(qir_major_version);
    QIREE_EXTRACT(qir_minor_version);
    QIREE_EXTRACT(dynamic_qubit_management);
    QIREE_EXTRACT(dynamic_result_management);

    return flags;
}

//---------------------------------------------------------------------------//
}  // namespace qiree
