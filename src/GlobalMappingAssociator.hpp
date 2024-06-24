#ifndef GLOBAL_MAPPING_ASSOCIATOR_HPP
#define GLOBAL_MAPPING_ASSOCIATOR_HPP

#include <unordered_map>
#include <string>
#include <functional>
#include "ExecutionBackend.hpp"

struct Qubit {
  int index;
};

struct Result {
  int index;
};

int extractQubitIndex(Qubit *qubit);
int extractResultIndex(Result *result);

struct _qis_pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const;
};

class GlobalMappingAssociator {
public:
  GlobalMappingAssociator();
  void addMapping(const std::string &name, std::int16_t argCount,
                  void (*)(Qubit *));
  void addMapping(const std::string &name, std::int16_t argCount,
                  void (*)(double));
  void *getMapping(const std::string &name, std::int16_t argCount) const;

private:
  std::unordered_map<std::pair<std::string, std::int16_t>, void (*)(Qubit *),
                     _qis_pair_hash>
      quantum_operation_map;
  std::unordered_map<std::pair<std::string, std::int16_t>, void (*)(double),
                     _qis_pair_hash>
      quantum_rotation_map;
};

#endif