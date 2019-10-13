#include <aes_128.h>
#include <aes_ila.h>
#include "artifact_utility.h"
#include <env.h>

#include <ilang/util/log.h>
#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>

using namespace ilang;


int loglevel(int argc, char **argv) {
  for (int idx = 1; idx < argc; ++idx)
    if(std::string(argv[idx]) == "fulllog")
      return 0;
  return 2;
}

int main (int argc, char ** argv) {
  SetLogLevel(loglevel(argc,argv));
  int timeout = get_timeout(argc, argv);

  // extract the configurations
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  // build the aes model
  AES aes_ila_model;

  VerilogGeneratorBase::VlgGenConfig vlg_cfg;

  vtg_cfg.InvariantSynthesisReachableCheckKeepOldInvariant = false;
  vtg_cfg.InvariantSynthesisKeepMemory = false;
  vtg_cfg.InvariantCheckKeepMemory = false;
  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction
  vtg_cfg.MaxBound = 40;                // bound of BMC
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.VerificationSettingAvoidIssueStage = true;
  vtg_cfg.YosysSmtFlattenDatatype = false;
  vtg_cfg.YosysSmtFlattenHierarchy = true;
  vtg_cfg.YosysPath = YOSYSPath;
  vtg_cfg.CosaPyEnvironment = COSAEnvPath;
  vtg_cfg.CosaPath = COSAPath;
  //vtg_cfg.Z3Path = "/home/hongce/z3s/bin";
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
      VerilogVerificationTargetGenerator::synthesis_backend_selector::ABC, // synthesis backend: Z3
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  int n_cegar = 0;
  double t_eq = 0;
  double t_syn= 0;
  double t_total=0;
  bool succeed = true;
  set_timeout(timeout, OutputPath, &n_cegar, &t_syn, & t_eq);

  std::vector<std::string> to_drop_states = {
    "m1.encrypted_data_buf[127:0]",
    "m1.mem_data_buf[127:0]",
    "m1.data_out_reg[7:0]",
    "m1.aes_128_i.out_reg[127:0]",
    "m1.aes_reg_oplen_i.reg_out[15:0]",
    "m1.aes_reg_opaddr_i.reg_out[15:0]",
    "m1.aes_reg_key0_i.reg_out[127:0]",
  };

  do{
    vg.GenerateVerificationTarget();
    if(vg.RunVerifAuto("OPERATE")) {// the OPERATE 
      std::cerr << "No more Cex has been found! Cegar completes." << std::endl;
      break; // no more cex found
    }
    vg.ExtractVerificationResult();
    vg.CexGeneralizeRemoveStates(to_drop_states);
    vg.GenerateSynthesisTarget();
    if(vg.RunSynAuto()) {
      std::cerr << "Cex is reachable! Cegar failed" << std::endl;

      break; // cex is really reachable!!!
    }
    vg.ExtractSynthesisResult();
    vg.GenerateInvariantVerificationTarget();

    auto design_stat = vg.GetDesignStatistics();
    t_eq = design_stat.TimeOfEqCheck;
    t_syn = design_stat.TimeOfInvSyn;
    t_total = design_stat.TimeOfEqCheck + design_stat.TimeOfInvSyn;
    n_cegar ++;

  }while(not vg.in_bad_state());

  vg.GetInvariants().ExportToFile(OutputPath+"inv.txt",false);
  set_result(OutputPath, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);

  return 0;
}
