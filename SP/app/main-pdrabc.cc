#include <sp_ila.h>
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
  cfg.AbcMinimizeInv = false;
  cfg.AbcUseCorr = false;
  cfg.CosaSolver = "btor";

  auto dirName = std::string("../");
  auto outDir  = dirName + "verification/PdrAbc/";

  int n_cegar = 0;
  double t_eq = 0;
  double t_syn= 0;
  double t_total=0;
  bool succeed = true;
  set_timeout(timeout, outDir, &n_cegar, &t_syn, & t_eq);

  InvariantSynthesizerCegar vg(
      {},                          // no include
      {dirName + "verilog/simple_pipe.v"}, //
      "pipeline_v",                // top_module_name
      dirName + "rfmap/vmap.json", // variable mapping
      dirName + "rfmap/cond-noinv.json", outDir, ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      VerilogVerificationTargetGenerator::synthesis_backend_selector::ABC,
      cfg);

  do{
    vg.GenerateVerificationTarget();
    if(vg.RunVerifAuto("ADD")) // the ADD
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
    n_cegar ++;
      
  }while(not vg.in_bad_state());

  vg.GetInvariants().ExportToFile(outDir+"inv.txt",false);
  set_result(outDir, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);

  return 0;
}