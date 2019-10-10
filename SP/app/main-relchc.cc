#include <artifact_utility.h>
#include <sp_ila.h>
#include <env.h>

#include <ilang/util/fs.h>
#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>

using namespace ilang;

int main (int argc, char ** argv) {

  int timeout = get_timeout(argc, argv);

  auto ila_model = BuildModel();

  VerilogVerificationTargetGenerator::vtg_config_t cfg;

  cfg.VerificationSettingAvoidIssueStage = true;
  cfg.CosaAddKeep = false;
  cfg.YosysPath = YOSYSPath;

  auto dirName = std::string("../");
  auto outDir  = dirName + "verification/RelChc/";

  VerilogVerificationTargetGenerator vg(
      {},                          // no include
      {dirName + "verilog/simple_pipe.v"}, //
      "pipeline_v",                // top_module_name
      dirName + "rfmap/vmap.json", // variable mapping
      dirName + "rfmap/cond-noinv.json", outDir, ila_model.get(),
      VerilogVerificationTargetGenerator::backend_selector::YOSYS,
      cfg);

  vg.GenerateTargets();

  // os_portable_execute_shell({"bash",outDir+"INC/run.sh"}, outDir + "yosys-log.txt" , redirect_t::BOTH, 0);
  auto res = os_portable_execute_shell({"z3",outDir+"ADD/wrapper.smt2"}, outDir + "eqcheck-invsyn.txt" , redirect_t::BOTH, timeout);
  // then let's run it
  
  set_result(outDir, !res.timeout && res.subexit_normal, res.seconds, 0,0,0);

  return 0;
}