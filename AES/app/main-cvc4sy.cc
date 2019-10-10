#include <aes_128.h>
#include <aes_ila.h>
#include <env.h>
#include "artifact_utility.h"

#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>
#include <ilang/vtarget-out/inv-syn/sygus/sim_trace_extract.h>

const char * restext[] = {"Proved", "Invalid", "Unknown"};

using namespace ilang;

int main (int argc, char ** argv) {

  int timeout = get_timeout(argc, argv);

  // extract the configurations
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  // build the aes model
  AES aes_ila_model;

  VerilogGeneratorBase::VlgGenConfig vlg_cfg;

  vtg_cfg.InvariantSynthesisReachableCheckKeepOldInvariant = true;
  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction
  vtg_cfg.MaxBound = 100;                // bound of BMC
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.VerificationSettingAvoidIssueStage = true;
  vtg_cfg.YosysSmtFlattenDatatype = true;
  vtg_cfg.YosysSmtFlattenHierarchy = true;
  vtg_cfg.InvariantSynthesisKeepMemory = false;
  vtg_cfg.InvariantCheckKeepMemory = false;
  vtg_cfg.CosaPyEnvironment = COSAEnvPath;
  vtg_cfg.CosaPath = COSAPath;
  vtg_cfg.Cvc4Path = CVC4Path;
  vtg_cfg.CosaSolver = "btor";
  vtg_cfg.CosaFullTrace = false;
  vtg_cfg.SygusOptions.SygusPassInfo = 
    VerilogVerificationTargetGenerator::vtg_config_t::_sygus_options_t::TransferFunc;

  vtg_cfg.SygusOptions.UseArithmetics = vtg_cfg.SygusOptions.Level1;

  TraceDataPoints dp;
  
  std::vector<std::string> sygus_var_name = {
    "m1.aes_reg_state", "m1.byte_counter", "m1.aes_time_counter",
    "m1.uaes_ctr", "m1.block_counter", "m1.aes_reg_ctr_i.reg_out", "m1.operated_bytes_count"};

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/Cvc4Sy/";


  InvariantSynthesizerCegar vg(
      {}, // no include
      {VerilogPath + "aes_top.v",   VerilogPath + "reg2byte.v",
       VerilogPath + "reg16byte.v", VerilogPath + "reg32byte.v",
       VerilogPath + "reg256byte.v",
       VerilogPath + "aes_128_abs.v"},                // designs
      "aes_top",                                      // top_module_name
      RefrelPath + "ref-rel-var-map-uaes.json",       // variable mapping
      RefrelPath + "ref-rel-inst-cond-uaes.json",     // conditions of start/ready
      OutputPath,                                     // output path
      aes_ila_model.model.child(0).get(),                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      VerilogVerificationTargetGenerator::synthesis_backend_selector::CVC4, // synthesis backend: Z3
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration
  
  vg.SetSygusVarnameList(sygus_var_name);

  Cvc4Syntax syntax;
  syntax.ctrl_data_sep_width = 5;
  syntax.other_comp_sep_width = 5;
  syntax.arithmOp = {{128,{{"bvadd",2}}}};
  syntax.extractExtOp = {
    {16, {
      extract_op(15,0,128,16, {}),
      } }  };

  int n_cegar = 0;
  double t_eq = 0;
  double t_syn = 0;
  double t_total = 0;
  bool succeed = true;
  set_timeout(timeout, OutputPath, &n_cegar, &t_syn, & t_eq);

  do{
    std::cout << "Generate Verif Target\n";
    vg.GenerateVerificationTarget();
    if(vg.RunVerifAuto("OPERATE")) { // the ADD
      std::cout << "No cex found. CEGAR completes!" << std::endl;
      break; // no more cex found
    }
      
    std::cout << "EQ check cex found, query sygus for invariant candidate..."; std::cout.flush();
    vg.ExtractVerificationResult(); // also the cex
    
    vg.GenerateSynthesisTargetSygusTransFunc(syntax);
    vg.RunSynAuto();
    vg.ExtractSynthesisResult(); // if should be the correct invariants

      auto design_stat = vg.GetDesignStatistics();
      t_eq = design_stat.TimeOfEqCheck;
      t_syn = design_stat.TimeOfInvSyn;
      t_total = design_stat.TimeOfEqCheck + design_stat.TimeOfInvSyn;
      n_cegar ++;

  }while(true); // first generate enough guesses


  set_result(OutputPath, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);
  return 0;
}
