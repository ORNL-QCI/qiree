#ifndef XACC_QIRCOMPILER_HPP
#define XACC_QIRCOMPILER_HPP

#include "Compiler.hpp"

namespace xacc {

class QIRCompiler : public xacc::Compiler {

private: 

protected:
  
public:

  QIRCompiler();

  bool canParse(const std::string &src) override;
    
  std::shared_ptr<xacc::IR> compile(const std::string &src) override;
  void setExtraOptions(const HeterogeneousMap options) override {
  } // add extra options if needed

  const std::string translate(std::shared_ptr<CompositeInstruction> function) override;

  const std::string translate(std::shared_ptr<CompositeInstruction> program, HeterogeneousMap &options) override;

  const std::string name() const override { return "qir"; }

  const std::string description() const override { return ""; }

  virtual ~QIRCompiler() {}

};

} // namespace xacc

#endif
