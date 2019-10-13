#include <artifact_utility.h>
#include "riscvIla.hpp"
#include <env.h>

#include <ilang/util/log.h>
#include <ilang/util/fs.h>
#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>

using namespace ilang;


int main (int argc, char ** argv) {

  int timeout = get_timeout(argc, argv);

  riscvILA_user riscvILA(0);
  riscvILA.addInstructions(); // 37 base integer instructions

  VerilogGeneratorBase::VlgGenConfig vlg_cfg;
  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;

  vtg_cfg.PortDeclStyle = VlgVerifTgtGenBase::_vtg_config::NEW;
  vtg_cfg.MemAbsReadAbstraction = true;
  vtg_cfg.CosaSolver = "btor";
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.YosysPath = YOSYSPath;
  //vtg_cfg.VerificationSettingAvoidIssueStage = true;

  std::string RootPath = "..";

  std::string IncludePath = RootPath + "/verilog/";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/RelChc/";

  VerilogVerificationTargetGenerator vg(
      {},                            // no include
      { VerilogPath + "picorv32.v"}, // designs

      "picorv32",                                             // top_module_name
      RefrelPath + "vmap.json",            // variable mapping
      RefrelPath + "cond.json",          // conditions of start/ready
      OutputPath,                                            // output path
      riscvILA.model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::YOSYS, // backend: COSA
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.GenerateTargets();

  // os_portable_execute_shell({"bash",outDir+"INC/run.sh"}, outDir + "yosys-log.txt" , redirect_t::BOTH, 0);
  auto res = os_portable_execute_shell({"z3",OutputPath+"ADD/wrapper.smt2"}, OutputPath + "eqcheck-invsyn.txt" , redirect_t::BOTH, timeout);
  
  set_result(OutputPath, !res.timeout && res.subexit_normal, res.seconds, 0,0,0);

  return 0;
}
