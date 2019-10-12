#include "riscvIla.hpp"
#include "artifact_utility.h"
#include <env.h>

#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>

using namespace ilang;

int main (int argc, char ** argv) {

  int timeout = get_timeout(argc, argv);

  riscvILA_user riscvILA(0);
  riscvILA.addInstructions(); // 37 base integer instructions


  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;

  vtg_cfg.PortDeclStyle = VlgVerifTgtGenBase::_vtg_config::NEW;
  vtg_cfg.InvariantSynthesisReachableCheckKeepOldInvariant = false;
  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction
  vtg_cfg.MaxBound = 40;                // bound of BMC
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.CosaGenJgTesterScript = true;
  vtg_cfg.VerificationSettingAvoidIssueStage = true;
  vtg_cfg.YosysSmtFlattenDatatype = false;
  vtg_cfg.YosysSmtFlattenHierarchy = true;
  vtg_cfg.YosysPath = YOSYSPath;
  vtg_cfg.InvariantSynthesisKeepMemory = false;
  vtg_cfg.InvariantCheckKeepMemory = false;
  vtg_cfg.CosaPyEnvironment = COSAEnvPath;
  vtg_cfg.CosaPath = COSAPath;
  // vtg_cfg.Z3Path = "/home/hongce/z3s/bin"; // use the latest version?
  vtg_cfg.AbcPath = ABCPath;
  vtg_cfg.AbcUseGla = false; // gla will result in too coarse approximate 
  vtg_cfg.AbcUseAiger = true;
  vtg_cfg.AbcAssumptionStyle = vtg_cfg.AssumptionRegister; // you have to do it this way!
  vtg_cfg.CosaSolver = "btor";

  vlg_cfg.pass_node_name = true;  // whether to use node name in Verilog

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/PdrAbc/";

  InvariantSynthesizerCegar vg(
      {},                            // no include
      { VerilogPath + "picorv32.v"}, // designs

      "picorv32",                                             // top_module_name
      RefrelPath + "vmap.json",            // variable mapping
      RefrelPath + "cond.json",          // conditions of start/ready
      OutputPath,                                            // output path
      riscvILA.model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      VerilogVerificationTargetGenerator::synthesis_backend_selector::ABC, // synthesis backend: Z3
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.LoadDesignSmtInfo(RootPath+"/smt/__design_smt.smt2");
  // vg.LoadInvariantsFromFile(OutputPath+"inv_cegar_results_abc.txt");

  int n_cegar = 0;
  double t_eq = 0;
  double t_syn= 0;
  double t_total=0;
  bool succeed = true;
  set_timeout(timeout, OutputPath, &n_cegar, &t_syn, & t_eq);

  do{
    vg.GenerateVerificationTarget();
    if(vg.RunVerifAuto("ADD")) {// the OPERATE 
      std::cerr << "No more Cex has been found! Cegar completes." << std::endl;
      break; // no more cex found
    }
    vg.ExtractVerificationResult();
    vg.CexGeneralizeRemoveStates({"m1.trace_data", "m1.timer"});
    vg.GenerateSynthesisTarget();
    if(vg.RunSynAuto()) {
      std::cerr << "Cex is reachable! Cegar failed" << std::endl;
      succeed = false;
      break; // cex is really reachable!!!
    }
    vg.ExtractSynthesisResult();

    auto design_stat = vg.GetDesignStatistics();
    t_eq = design_stat.TimeOfEqCheck;
    t_syn = design_stat.TimeOfInvSyn;
    t_total = design_stat.TimeOfEqCheck + design_stat.TimeOfInvSyn;
    n_cegar ++;

  }while(not vg.in_bad_state());

  set_result(OutputPath, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);

  return 0;
}
