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
  cfg.YosysSmtFlattenDatatype = true; // let's test flatten datatype also
  cfg.YosysSmtFlattenHierarchy = true;
  cfg.YosysUndrivenNetAsInput = true;
  cfg.CosaPyEnvironment = "/home/hongce/cosaEnv/bin/activate";
  cfg.CosaPath = "/home/hongce/CoSA/";
  cfg.Z3Path = "N/A";
  cfg.FreqHornPath = "/home/hongce/ila/aeval/build/tools/bv/";
  cfg.FreqHornHintsUseCnfStyle = true;
  cfg.FreqHornOptions = {
    "--skip-cnf --skip-const-check --skip-stat-collect --ante-size 1 --conseq-size 1  --cnf cnt-no-group.cnf --use-arith-bvnot --no-const-enum-vars-on m1.v,m1.imp"};
  cfg.CosaSolver = "btor";


  set_result(outDir, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);
  return 0;
}