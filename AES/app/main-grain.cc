#include <aes_128.h>
#include <aes_ila.h>
#include <env.h>
#include "artifact_utility.h"

#include <ilang/util/fs.h>
#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>

using namespace ilang;

int main (int argc, char ** argv) {

  int timeout = get_timeout(argc, argv);

  // extract the configurations
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  // build the aes model
  AES aes_ila_model;

  VerilogGeneratorBase::VlgGenConfig vlg_cfg;

  vtg_cfg.InvariantSynthesisReachableCheckKeepOldInvariant = false;
  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction
  vtg_cfg.MaxBound = 40;                // bound of BMC
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.VerificationSettingAvoidIssueStage = true;
  vtg_cfg.YosysSmtFlattenDatatype = true;
  vtg_cfg.YosysSmtFlattenHierarchy = true;
  vtg_cfg.YosysUndrivenNetAsInput = true;
  vtg_cfg.CosaPyEnvironment = COSAEnvPath;
  vtg_cfg.CosaPath = COSAPath;
  vtg_cfg.FreqHornPath = FREQHORNPath; //"/home/hongce/ila/aeval/build/tools/bv/";
  vtg_cfg.FreqHornOptions = {"--const-bw=5 --ante-size=1 --conseq-size=1 --conseq-disj=1 --grammar-file=\"aes.gmr\""};
  vtg_cfg.FreqHornHintsUseCnfStyle = false;
  vtg_cfg.CosaSolver = "btor";

  vlg_cfg.pass_node_name = true;  // whether to use node name in Verilog

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/Grain/";

  int n_cegar = 0;
  double t_eq = 0;
  double t_syn = 0;
  double t_total = 0;
  bool succeed = true;
  set_timeout(timeout, OutputPath, &n_cegar, &t_syn, & t_eq);

  { // save grammar file
    std::string gmr = R"##(CTRL-STATE: S_m1.aes_reg_state, S_m1.byte_counter, S_m1.aes_time_counter
CTRL-OUT: S_m1.byte_counter, S_m1.aes_time_counter
DATA-OUT: S_m1.uaes_ctr, S_m1.block_counter
DATA-IN: S_m1.aes_reg_ctr_i.reg_out, S_m1.block_counter, S_m1.operated_bytes_count
)##";

    std::ofstream fout(OutputPath + "inv-syn/pipe.gmr");
    if (fout.is_open())
      fout << gmr;
    else
      succeed = false;
  } // save grammar file


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
      VerilogVerificationTargetGenerator::synthesis_backend_selector::FreqHorn, // synthesis backend: Z3
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration


  do{
    vg.GenerateVerificationTarget();
    if(vg.RunVerifAuto("OPERATE")) {// the OPERATE 
      std::cerr << "No more Cex has been found! Cegar completes." << std::endl;
      break; // no more cex found
    }
    vg.ExtractVerificationResult();
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

