#include <rc_ila.h>
#include "artifact_utility.h"

#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>


int main (int argc, char ** argv) {

  int timeout = get_timeout(argc, argv);

  auto ila_model = BuildModel();

  VerilogVerificationTargetGenerator::vtg_config_t cfg;
  cfg.InvariantSynthesisReachableCheckKeepOldInvariant = false;
  cfg.CosaAddKeep = false;
  cfg.VerificationSettingAvoidIssueStage = true;
  cfg.YosysSmtFlattenDatatype = false;
  cfg.YosysSmtFlattenHierarchy = true;
  cfg.CosaPyEnvironment = COSAEnvPath;
  cfg.CosaPath = COSAPath;
  cfg.AbcPath = ABCPath;
  cfg.AbcUseGla = true;
  cfg.AbcUseAiger = true;
  cfg.AbcUseCorr = false;
  cfg.CosaSolver = "btor";

  auto dirName = std::string("../");
  auto outDir  = dirName + "verification/PdrAbc/";

  int n_cegar = 0;
  double t_eq;
  double t_syn;
  bool succeed = true;
  set_timeout(timeout, outDir, &n_cegar, &t_syn, & t_eq);

  InvariantSynthesizerCegar vg(
      {},                          // no include
      {dirName + "verilog/opposite.v"}, //
      "opposite",                // top_module_name
      dirName + "refinement/vmap.json", // variable mapping
      dirName + "refinement/cond.json", outDir, ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      VerilogVerificationTargetGenerator::synthesis_backend_selector::ABC,
      cfg);

  do{
    vg.GenerateVerificationTarget();
    if(vg.RunVerifAuto("INC")) // the ADD
      break; // no more cex found
    vg.ExtractVerificationResult();
    vg.GenerateSynthesisTarget();
    if(vg.RunSynAuto()) {
        succeed = false;
        break; 
    }
    vg.ExtractSynthesisResult();

    auto design_stat = vg.GetDesignStatistics();
    t_eq = design_stat.TimeOfEqCheck;
    t_syn = design_stat.TimeOfInvSyn;
    t_total = design_stat.TimeOfEqCheck + design_stat.TimeOfInvSyn;
      
  }while(not vg.in_bad_state());

  set_result(outDir, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);

  return 0;
}