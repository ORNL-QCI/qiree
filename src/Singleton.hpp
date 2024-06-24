#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <xacc/xacc.hpp>

class Singleton {
public:
    static Singleton& Instance() {
        static Singleton instance;
        return instance;
    }

    std::shared_ptr<xacc::AcceleratorBuffer>& getBuffer() { return buffer; }
    std::shared_ptr<xacc::Accelerator>& getAccelerator() { return accelerator; }
    std::shared_ptr<xacc::CompositeInstruction>& getAnsatz() { return ansatz; }
    std::shared_ptr<xacc::IRProvider>& getProvider() { return provider; }
    std::shared_ptr<xacc::CompositeInstruction>& getIfStmt() { return ifStmt; }

    void setIfStmtExecuted(bool value) { 
        ifStmtExecuted = value; 
        }

    bool getIfStmtExecuted() const{
        return ifStmtExecuted; 
        }

    // create a function that will keep track of qubit indices and their corresponding result indices
    // this function will be called by the quantum__qis__measure__body function
    void addQubitResultMapping(int qubitIndex, int resultIndex){
        qubitResultMap.insert(std::make_pair(qubitIndex, resultIndex));
    }

    // create a function that will return the qubit index for a given result index
    int getQubitIndex(int resultIndex){
        return qubitResultMap.find(resultIndex)->second;
    }
    // create a function that will return the result index for a given qubit index
    int getResultIndex(int qubitIndex){
        return qubitResultMap.find(qubitIndex)->first;
    }


private:
    Singleton() = default;
    ~Singleton() = default;

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    std::shared_ptr<xacc::AcceleratorBuffer> buffer;
    std::shared_ptr<xacc::Accelerator> accelerator;
    std::shared_ptr<xacc::CompositeInstruction> ansatz;
    std::shared_ptr<xacc::IRProvider> provider;
    std::shared_ptr<xacc::CompositeInstruction> ifStmt;
    bool ifStmtExecuted = false;
    std::unordered_map<int, int> qubitResultMap;

};

#endif // SINGLETON_HPP
