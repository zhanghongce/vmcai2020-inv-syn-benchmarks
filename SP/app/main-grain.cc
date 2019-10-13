#include <sp_ila.h>
#include <env.h>
#include "artifact_utility.h"

#include <ilang/util/log.h>
#include <ilang/util/fs.h>
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
  cfg.InvariantSynthesisReachableCheckKeepOldInvariant = true;
  cfg.CosaAddKeep = false;
  cfg.VerificationSettingAvoidIssueStage = true;
  cfg.YosysSmtFlattenDatatype = true;
  cfg.YosysSmtFlattenHierarchy = true;
  cfg.YosysUndrivenNetAsInput = true;
  cfg.CosaPyEnvironment = COSAEnvPath;
  cfg.CosaPath = COSAPath;
  cfg.Z3Path = "N/A";
  cfg.FreqHornPath = FREQHORNPath;
  cfg.FreqHornHintsUseCnfStyle = true;

  std::vector<std::string> fh_options = {
    "--ante-size","1","--conseq-size","1","--find-one",
    "--skip-cnf --skip-const-check --skip-stat-collect","--ctrl-no-cross-var-eq",
    "--cti-prune --no-merge-cti --cnf pipe.cnf --grammar pipe.gmr"};

  //std::vector<std::string> fh_options = {
  //  "--ante-size","1","--conseq-size","1", "--2-chance",
  //  "--skip-cnf --skip-const-check --skip-stat-collect","--ctrl-no-cross-var-eq",
  //  "--no-merge-cti --cnf pipe-no-group.cnf --grammar pipe.gmr"};

  cfg.FreqHornOptions = fh_options;
  cfg.CosaSolver = "btor";


  auto dirName = std::string("../");
  auto outDir  = dirName + "verification/Grain/";

  int n_cegar = 1;
  double t_eq = 0;
  double t_syn = 0;
  double t_total = 0;
  bool succeed = true;
  set_timeout(timeout, outDir, &n_cegar, &t_syn, & t_eq);
  
  InvariantInCnf var_in_cnf;
  { // save grammar file
    os_portable_mkdir(outDir + "inv-syn");
    InvariantInCnf::clause cl;
    InvariantInCnf::VarsToClause( {
      "m1.reg_0_w_stage","m1.reg_1_w_stage", "m1.reg_2_w_stage", "m1.reg_3_w_stage",
      "m1.id_ex_reg_wen","m1.ex_wb_reg_wen", "m1.id_ex_rd", "m1.ex_wb_rd"
      } , cl);
    var_in_cnf.InsertClause(cl);
    std::ofstream fout(outDir + "inv-syn/pipe.cnf");
    if (fout.is_open())
      var_in_cnf.ExportInCnfFormat(fout);
    else
      succeed = (false);
  }
  {
    std::string gmr = R"##(CTRL-STATE: m1.reg_0_w_stage, m1.reg_1_w_stage, m1.reg_2_w_stage, m1.reg_3_w_stage
CTRL-OUT: m1.id_ex_reg_wen, m1.ex_wb_reg_wen 
DATA-OUT: m1.id_ex_rd, m1.ex_wb_rd
VAR-GROUP: m1.id_ex_reg_wen, m1.id_ex_rd
VAR-GROUP: m1.ex_wb_reg_wen, m1.ex_wb_rd
)##";

    std::ofstream fout(outDir + "inv-syn/pipe.gmr");
    if (fout.is_open())
      fout << gmr;
    else
      succeed = (false);
  } // save grammar file
  
  InvariantSynthesizerCegar vg(
      {},                          // no include
      {dirName + "verilog/simple_pipe.v"}, //
      "pipeline_v",                // top_module_name
      dirName + "rfmap/vmap.json", // variable mapping
      dirName + "rfmap/cond-noinv.json", outDir, ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::COSA,
      VerilogVerificationTargetGenerator::synthesis_backend_selector::FreqHorn,
      cfg);


    do{
      vg.GenerateVerificationTarget();
      if(vg.RunVerifAuto("ADD")) // the ADD
        break; // no more cex found
      vg.ExtractVerificationResult();

      unsigned conseq_size = 1;
      
  pipe_conseq_retry:
      fh_options[3] = std::to_string(conseq_size);
      vg.ChangeFreqHornSyntax(fh_options);

      vg.GenerateSynthesisTarget();
      if(vg.RunSynAuto()) {
        conseq_size ++;
        std::cout << "Conseq size increase to #" << conseq_size << std::endl;
        if (conseq_size < 3)
          goto pipe_conseq_retry;
        // else
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