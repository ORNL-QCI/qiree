#include "qir_compiler.hpp"
#include "xacc.hpp"
#include "xacc_plugin.hpp"

namespace xacc {

static std::vector<std::string> builtins{
}; // add built-in gates here

static std::map<std::string, std::string> missing_builtins{
}; // add custom gates

QIRCompiler::QIRCompiler() {}

bool QIRCompiler::canParse(const std::string &src) {
} // some function

std::shared_ptr<IR> QIRCompiler::compile(const std::string &src) {
  // IF src contains typical xacc quantum kernel prototype, then take
  // buffers names and add qreg declarations before parsing
  // IF it does not, then assume qreg decls already declared.
} // build function to add library here

const std::string QIRCompiler::translate(std::shared_ptr<xacc::CompositeInstruction> function) {
} // translate

const std::string QIRCompiler::translate(std::shared_ptr<CompositeInstruction> program, HeterogeneousMap &options) {
} // translate

} // namespace xacc