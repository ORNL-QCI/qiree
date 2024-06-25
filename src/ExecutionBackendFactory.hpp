#ifndef EXECUTION_BACKEND_FACTORY_HPP
#define EXECUTION_BACKEND_FACTORY_HPP

#include "ExecutionBackend.hpp"

/**
 * Simple Factory for new implementations.
 * New implementations need only to:
 *
 * 1) include header with implementation;
 * 2) accordingly adjust definition of createExecutionBackend().
 */
class ExecutionBackendFactory {
public:
  static ExecutionBackend &createExecutionBackend();
};

#endif