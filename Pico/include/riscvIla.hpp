/********************

 RISC-V ILA

********************/

#ifndef __RISCV_ILA_HPP__
#define __RISCV_ILA_HPP__

#include "encoding.hpp"
#include <ilang/ilang++.h>
#include <set>
#include <vector>

namespace ilang {

class riscvILA_user {

public:
  Ila model;
  
protected:

  ExprRef pc;
  ExprRef dmem;
  ExprRef imem;
  ExprRef count_cycle;
  ExprRef count_instr;
  std::vector<ExprRef> GPR; // R0-R31

  std::set<std::string> Instrs;
  std::map<std::string, InstrRef> InstrMap;

  ExprRef inst;

  ExprRef opcode;
  ExprRef rd;
  ExprRef rs1;
  ExprRef rs2;
  ExprRef funct3;
  ExprRef funct7;
  ExprRef funct12;
  ExprRef immI;
  ExprRef immS;
  ExprRef immB;
  ExprRef immU;
  ExprRef immJ;
  ExprRef csr_index;

protected:
  ExprRef indexIntoGPR(const ExprRef& idxBits);
  void UpdateGPR(InstrRef& inst, const ExprRef& idxBits, const ExprRef& val);

  ExprRef bv(int val) { return BvConst(val, XLEN); }
  ExprRef zext(const ExprRef& v) { return ZExt(v, XLEN); }
  ExprRef sext(const ExprRef& v) { return SExt(v, XLEN); }

  ExprRef getSlice(const ExprRef& word, const ExprRef& lowBits, int width,
                   bool unSigned);

  // privileged model will overload these to insert their address translation
  virtual ExprRef FetchFromMem(const ExprRef& m, const ExprRef& addr) {
    return Load(m, addr);
  }
  virtual ExprRef LoadFromMem(const ExprRef& m, const ExprRef& addr) {
    return Load(m, addr);
  }
  virtual ExprRef StoreToMem(const ExprRef& m, const ExprRef& addr,
                             const ExprRef& data) {
    return Store(m, addr, data);
  }

public:
  riscvILA_user(int pc_init_val);
  void addInstructions();
};

class riscvILA_machine : public riscvILA_user {};

}; // namespace ilang

#endif
