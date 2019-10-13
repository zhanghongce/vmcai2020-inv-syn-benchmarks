#include <rc_ila.h>
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
  cfg.YosysSmtFlattenDatatype = false; // let's test flatten datatype also
  cfg.YosysSmtFlattenHierarchy = true;
  cfg.YosysPath = YOSYSPath;
  cfg.CosaPyEnvironment = COSAEnvPath;
  cfg.CosaPath = COSAPath;
  //cfg.Z3Path = "/home/vmcai2020/z3s/bin/";
  cfg.CosaSolver = "btor";

  auto dirName = std::string("../");
  auto outDir  = dirName + "verification/PdrChc/";

  InvariantSynthesizerCegar vg(
      {},                          // no include
      {dirName + "verilog/opposite.v"}, //
      "opposite",                // top_module_name
      dirName + "refinement/vmap.json", // variable mapping
      dirName + "refinement/cond.json", outDir, ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      VerilogVerificationTargetGenerator::synthesis_backend_selector::Z3,
      cfg);

  int n_cegar = 0;
  double t_eq = 0;
  double t_syn= 0;
  double t_total = 0;
  bool succeed = true;
  set_timeout(timeout, outDir, &n_cegar, &t_syn, & t_eq);

    do{

      vg.GenerateVerificationTarget();
      if(vg.RunVerifAuto("INC")) // the ADD
        break; // no more cex found
      vg.ExtractVerificationResult();
      vg.GenerateSynthesisTarget(); // you will need fp engine
      if(vg.RunSynAuto()) {
        succeed = false;
        break; 
      }
      vg.ExtractSynthesisResult(); // very weired, it throw away something in arg

      auto design_stat = vg.GetDesignStatistics();
      t_eq = design_stat.TimeOfEqCheck;
      t_syn = design_stat.TimeOfInvSyn;
      t_total = design_stat.TimeOfEqCheck + design_stat.TimeOfInvSyn;
      n_cegar ++ ;
    }while(not vg.in_bad_state());
  
  if (vg.in_bad_state())
    succeed = false;

  vg.GetInvariants().ExportToFile(outDir+"inv.txt",false);
  set_result(outDir, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);

  return 0;
}