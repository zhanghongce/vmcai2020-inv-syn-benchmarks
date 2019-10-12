#include <sp_ila.h>
#include <env.h>
#include "artifact_utility.h"

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
  cfg.YosysSmtFlattenDatatype = true; // if not using transfunc it does not matter
  cfg.YosysSmtFlattenHierarchy = true;
  cfg.YosysPath = YOSYSPath;
  cfg.CosaPyEnvironment = COSAEnvPath;
  cfg.CosaPath = COSAPath;
  cfg.Cvc4Path = CVC4Path;
  cfg.Cvc4Path = CVC4Path;
  cfg.CosaSolver = "btor";
  cfg.SygusOptions.SygusPassInfo = 
    VerilogVerificationTargetGenerator::vtg_config_t::_sygus_options_t::TransferFunc;

  //cfg.Z3Path = "";
  TraceDataPoints dp;
  std::vector<std::string> sygus_var_name = {
    "m1.reg_0_w_stage", "m1.reg_1_w_stage", "m1.reg_2_w_stage", "m1.reg_3_w_stage",
    "m1.id_ex_reg_wen", "m1.id_ex_rd", "m1.ex_wb_reg_wen", "m1.ex_wb_rd" };



  auto dirName = std::string("../");
  auto outDir  = dirName + "verification/Cvc4Sy/";

  int n_cegar = 0;
  double t_eq = 0;
  double t_syn = 0;
  double t_total = 0;
  bool succeed = true;
  set_timeout(timeout, outDir, &n_cegar, &t_syn, & t_eq);

  InvariantSynthesizerCegar vg(
      {},                          // no include
      {dirName + "verilog/simple_pipe.v"}, //
      "pipeline_v",                // top_module_name
      dirName + "rfmap/vmap.json", // variable mapping
      dirName + "rfmap/cond-noinv.json", outDir, ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      VerilogVerificationTargetGenerator::synthesis_backend_selector::CVC4,
      cfg);


  Cvc4Syntax syntax;
  syntax.ctrl_data_sep_width = 5;
  syntax.other_comp_sep_width = 5;

    do{
      vg.GenerateVerificationTarget();
      if(vg.RunVerifAuto("ADD")) // the INC
        break; // no more cex found
      vg.ExtractVerificationResult();
      //vg.SetInitialDatapoint(dp);
      vg.SetSygusVarnameList(sygus_var_name);
      vg.GenerateSynthesisTargetSygusTransFunc(syntax);
      vg.RunSynAuto();
      vg.ExtractSynthesisResult(); // if should be the correct invariants

      auto design_stat = vg.GetDesignStatistics();
      t_eq = design_stat.TimeOfEqCheck;
      t_syn = design_stat.TimeOfInvSyn;
      t_total = design_stat.TimeOfEqCheck + design_stat.TimeOfInvSyn;
      n_cegar ++;

    }while(not vg.in_bad_state());
  vg.GenerateInvariantVerificationTarget(); // finally we revalidate the result

  set_result(outDir, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);
  return 0;
}
