#ifndef SINGLETON_HANDLE_HPP
#define SINGLETON_HANDLE_HPP

#include "ExecutionBackend.hpp"

/**
 * Proxy to access methods in the chosen singleton backend.
 * Backends need to be implemented as singleton.
 */

class SingletonHandle : public ExecutionBackend {
private:
  /**
   * @brief Reference to the backend plugin.
   */
  ExecutionBackend &_backendPlugin;

  /**
   * @brief Singleton handle constructor.
   *
   * @param plugin Reference to the backend plugin.
   */
  SingletonHandle(ExecutionBackend &plugin);

public:
  static SingletonHandle &Instance();
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