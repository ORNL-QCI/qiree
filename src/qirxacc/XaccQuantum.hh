#ifndef XACC_EXECUTION_BACKEND_HPP
#define XACC_EXECUTION_BACKEND_HPP

#include "ExecutionBackend.hpp"
#include <xacc/xacc.hpp>

class XACCExecutionBackend : public ExecutionBackend {
private:
  XACCExecutionBackend() {}

  std::shared_ptr<xacc::AcceleratorBuffer> buffer;
  std::shared_ptr<xacc::Accelerator> accelerator;
  std::shared_ptr<xacc::CompositeInstruction> ansatz;
  std::shared_ptr<xacc::IRProvider> provider;
  std::shared_ptr<xacc::CompositeInstruction> ifStmt;

public:
  static XACCExecutionBackend &Instance();
  void initialize(const std::string, const int, const int) override;
  void finalize() override;
  void addInstruction(const std::string &,
                      const std::vector<unsigned long>) override;
  void addInstruction(const std::string &, const std::vector<unsigned long>,
                      const double) override;
  void addMeasure(const int, const int) override;
  int getQubitOutput(const int, const std::string) override;
  void execute() override;
  void addIfStmt(const int) override;
};

#endif