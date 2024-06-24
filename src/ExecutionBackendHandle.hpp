// SingletonHandle.hpp
#ifndef SINGLETON_HANDLE_HPP
#define SINGLETON_HANDLE_HPP

#include "ExecutionBackend.hpp"

class SingletonHandle : public ExecutionBackend {
private:
  ExecutionBackend &backendPlugin;
  SingletonHandle(ExecutionBackend &plugin);

public:
  static SingletonHandle &Instance();
  void initialize(const std::string, const int, const int) override;
  void finalize() override;
  void addInstructionToAnsatz(const std::string &,
                              const std::vector<unsigned long>) override;
  void addParametrizedInstructionToAnsatz(const std::string &,
                                          std::vector<unsigned long>,
                                          double) override;
  void addMeasure(const int, const int) override;
  int getOutputQubitIndex(int, std::string) override;
  void execute() override;
  void addIfStmt(int) override;
};

#endif