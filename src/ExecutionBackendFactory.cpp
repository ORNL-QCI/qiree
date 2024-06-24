#include "ExecutionBackendFactory.hpp"
#include "XACCExecutionBackend.hpp"

ExecutionBackend &ExecutionBackendFactory::createExecutionBackend() {
  return XACCExecutionBackend::Instance();
}