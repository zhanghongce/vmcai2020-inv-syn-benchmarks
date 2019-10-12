#include <artifact_utility.h>
#include <aes_128.h>
#include <aes_ila.h>
#include <env.h>

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

  // set ilang option, operators like '<' will refer to unsigned arithmetics
  SetUnsignedComparison(true); 

  // build the aes model
  AES aes_ila_model;

  VerilogVerificationTargetGenerator::vtg_config_t cfg;
  cfg.CosaAddKeep = false;
  cfg.MemAbsReadAbstraction = true; // enable read abstraction
  cfg.YosysUndrivenNetAsInput = true;
  cfg.YosysPath = YOSYSPath;

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/RelChc/";;
  VerilogVerificationTargetGenerator vg(
      {},                          // no include
      {VerilogPath + "aes_top.v",   VerilogPath + "reg2byte.v",
       VerilogPath + "reg16byte.v", VerilogPath + "reg32byte.v",
       VerilogPath + "reg256byte.v",
       VerilogPath + "aes_128_abs.v"},  
      "aes_top",                                      // top_module_name
      RefrelPath + "ref-rel-var-map-uaes.json",       // variable mapping
      RefrelPath + "ref-rel-inst-cond-uaes.json",     // conditions of start/ready
      OutputPath,                                     // output path
      aes_ila_model.model.child(0).get(),                           // model
      VerilogVerificationTargetGenerator::backend_selector::YOSYS,
      cfg);

  vg.GenerateTargets();

  // os_portable_execute_shell({"bash",outDir+"INC/run.sh"}, outDir + "yosys-log.txt" , redirect_t::BOTH, 0);
  auto res = os_portable_execute_shell({"z3",OutputPath+"OPERATE/wrapper.smt2"}, OutputPath + "eqcheck-invsyn.txt" , redirect_t::BOTH, timeout);
  
  set_result(OutputPath, !res.timeout && res.subexit_normal, res.seconds, 0,0,0);

  return 0;
}
