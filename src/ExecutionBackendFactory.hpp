#ifndef EXECUTION_BACKEND_FACTORY_HPP
#define EXECUTION_BACKEND_FACTORY_HPP

#include "ExecutionBackend.hpp"

class ExecutionBackendFactory {
public:
  static ExecutionBackend &createExecutionBackend();
};

#endif