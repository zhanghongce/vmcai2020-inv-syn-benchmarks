#include "riscvIla.hpp"
#include <env.h>
#include "artifact_utility.h"

#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>
#include <ilang/vtarget-out/inv-syn/sygus/sim_trace_extract.h>

const char * restext[] = {"Proved", "Invalid", "Unknown"};

using namespace ilang;

void InsertStates(const InvariantInCnf & inv_cnf, std::vector<std::string> & vs) {
  std::set<std::string> name_set;
  for (auto &&  string_lit_vec_pair : inv_cnf.GetCnfs() ) {
    for (auto && lit : string_lit_vec_pair.second) {
      const auto & vn = std::get<1>(lit);
      if (name_set.find(vn) != name_set.end()) continue;
      vs.push_back(vn);      
    }
  }
}

int main (int argc, char ** argv) {

  int timeout = get_timeout(argc, argv);

  riscvILA_user riscvILA(0);
  riscvILA.addInstructions(); // 37 base integer instructions

  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;

  vtg_cfg.PortDeclStyle = VlgVerifTgtGenBase::_vtg_config::NEW;
  vtg_cfg.InvariantSynthesisReachableCheckKeepOldInvariant = true;
  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction
  vtg_cfg.MaxBound = 40;                // bound of BMC
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.CosaGenJgTesterScript = true;
  vtg_cfg.VerificationSettingAvoidIssueStage = true;
  vtg_cfg.YosysPath = YOSYSPath;
  vtg_cfg.YosysSmtFlattenDatatype = false;
  vtg_cfg.YosysSmtFlattenHierarchy = true;
  vtg_cfg.InvariantSynthesisKeepMemory = false;
  vtg_cfg.InvariantCheckKeepMemory = false;
  vtg_cfg.CosaPyEnvironment = COSAEnvPath;
  vtg_cfg.CosaPath = COSAPath;
  //vtg_cfg.Z3Path = "/home/hongce/z3s/bin"; // use the latest version?
  vtg_cfg.Cvc4Path = CVC4Path;
  vtg_cfg.CosaSolver = "btor";
  vtg_cfg.SygusOptions.SygusPassInfo = 
    VerilogVerificationTargetGenerator::vtg_config_t::_sygus_options_t::TransferFunc;
  vtg_cfg.SygusOptions.UseArithmetics = vtg_cfg.SygusOptions.Level1;
  vtg_cfg.SygusOptions.AllConstantUnderThisSize = 5;

  Cvc4Syntax syntax;
  syntax.ctrl_data_sep_width = 8;
  syntax.other_comp_sep_width = 8;
  syntax.nums = { {8,{0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80}} };
  syntax.extractExtOp = {
    {1, {
      extract_op(31,31,32,1),
      extract_op(20,20,32,1),
      extract_op(15,15,32,1),
      extract_op(7,7,32,1)
      } },
    {11, {extract_op(30,20,32,11)} },
    {12, {extract_op(31,20,32,12)} },
    {8, {extract_op(19,12,32,8)} },
    {6, {extract_op(30,25,32,6)} },
    {4, {
      extract_op(11,8,32,4),
      extract_op(19,16,32,4),
      extract_op(24,21,32,4)} },
    {31, {extract_op(31,1,32,31)} } 
  };

  std::vector<std::string> sygus_var_name;


  vlg_cfg.pass_node_name = true;  // whether to use node name in Verilog

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/Cvc4Sy/";

  InvariantSynthesizerCegar vg(
      {},                            // no include
      { VerilogPath + "picorv32.v"}, // designs

      "picorv32",                                             // top_module_name
      RefrelPath + "vmap.json",            // variable mapping
      RefrelPath + "cond.json",          // conditions of start/ready
      OutputPath,                                            // output path
      riscvILA.model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      VerilogVerificationTargetGenerator::synthesis_backend_selector::Z3, // synthesis backend: Z3
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.LoadDesignSmtInfo(RootPath+"/smt/__design_smt.smt2");

  int n_cegar = 0;
  double t_eq = 0;
  double t_syn = 0;
  double t_total = 0;
  bool succeed = true;
  set_timeout(timeout, OutputPath, &n_cegar, &t_syn, & t_eq);


  do{
    vg.GenerateVerificationTarget();
    if(vg.RunVerifAuto("ADD")) {// the OPERATE 
      std::cerr << "No more Cex has been found! Cegar completes." << std::endl;
      break; // no more cex found
    }
    vg.ExtractVerificationResult();
    vg.CexGeneralizeRemoveStates({
      "m1.trace_data[35:0]", 
      "m1.timer[31:0]", 
      "m1.count_cycle[63:0]", 
      "m1.count_instr[63:0]",
      "m1.alu_out_q[31:0]", 
      "m1.mem_rdata_q[31:0]"});
    vg.GenerateSynthesisTarget();
    if(vg.RunSynAuto()) {
      std::cerr << "Cex is reachable! Cegar failed" << std::endl;
      succeed = false;
      break; // cex is really reachable!!!
    }
    vg.ExtractSynthesisResult();
    { // word level enhancement
      /// The incremental cnf
      InvariantInCnf incremental_cnf;
      //vg.ChangeFreqHornSyntax( fh_configs[cegar_idx] );
      vg.ExtractInvariantVarForEnhance(vg.GetInvariants().NumInvariant() - 1, incremental_cnf, true,
         {"m1.trace_data", "m1.timer", "m1.count_cycle", "m1.count_instr", "m1.alu_out_q", "m1.mem_rdata_q",
          "m1.cpuregs","m1.reg_op2","m1.reg_op1", "m1.reg_pc" }
        );
      sygus_var_name.clear();
      InsertStates(incremental_cnf,sygus_var_name);
      vg.RemoveInvariantsByIdx(vg.GetInvariants().NumInvariant() - 1);
      vg.SetSygusVarnameList(sygus_var_name);
      vg.GenerateSynthesisTargetSygusTransFunc(syntax);
      vg.RunSynAuto();
      vg.ExtractSynthesisResult(); // if should be the correct invariants

    }

      auto design_stat = vg.GetDesignStatistics();
      t_eq = design_stat.TimeOfEqCheck;
      t_syn = design_stat.TimeOfInvSyn;
      t_total = design_stat.TimeOfEqCheck + design_stat.TimeOfInvSyn;
      n_cegar ++;

  }while(not vg.in_bad_state());

  set_result(OutputPath, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);
  return 0;
}
////////////////////////////////////////////////////////////////////////

