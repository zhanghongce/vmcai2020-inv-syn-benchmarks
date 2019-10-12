#include "riscvIla.hpp"
#include <env.h>
#include "artifact_utility.h"

#include <ilang/util/fs.h>
#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>

using namespace ilang;

void InsertStates(InvariantInCnf & inv_cnf, const std::vector<std::string> & vs) {
  InvariantInCnf::clause cl;
  for (auto &&  v : vs )
    cl.push_back({false, v, 0});
  inv_cnf.InsertClause(cl);
}


int main (int argc, char ** argv) {

  int timeout = get_timeout(argc, argv);

  riscvILA_user riscvILA(0);
  riscvILA.addInstructions(); // 37 base integer instructions

  VerilogVerificationTargetGenerator::vtg_config_t vtg_cfg;
  VerilogGeneratorBase::VlgGenConfig vlg_cfg;

  vtg_cfg.PortDeclStyle = VlgVerifTgtGenBase::_vtg_config::NEW;
  vtg_cfg.InvariantSynthesisReachableCheckKeepOldInvariant = false;
  vtg_cfg.MemAbsReadAbstraction = true; // enable read abstraction
  vtg_cfg.MaxBound = 40;                // bound of BMC
  vtg_cfg.CosaAddKeep = false;
  vtg_cfg.CosaGenJgTesterScript = true;
  vtg_cfg.VerificationSettingAvoidIssueStage = true;
  vtg_cfg.YosysSmtFlattenDatatype = false;
  vtg_cfg.YosysSmtFlattenHierarchy = true;
  vtg_cfg.YosysPath = YOSYSPath;
  vtg_cfg.InvariantSynthesisKeepMemory = false;
  vtg_cfg.InvariantCheckKeepMemory = false;
  vtg_cfg.CosaPyEnvironment = COSAEnvPath;
  vtg_cfg.CosaPath = COSAPath;
  //vtg_cfg.Z3Path = "/home/hongce/z3s/bin"; // use the latest version?
  vtg_cfg.AbcPath = ABCPath;
  vtg_cfg.AbcUseGla = false; // gla will result in too coarse approximate 
  vtg_cfg.AbcUseAiger = true;
  vtg_cfg.AbcAssumptionStyle = vtg_cfg.AssumptionRegister; // you have to do it this way!
  vtg_cfg.CosaSolver = "btor";

  vtg_cfg.FreqHornPath = FREQHORNPath;


  int n_cegar = 0;
  double t_eq = 0;
  double t_syn = 0;
  double t_total = 0;
  bool succeed = true;

  const std::vector<std::string> base_fh_config = {
     "--ante-size", "1", "--conseq-size", "1", "--grammar pico.gmr",
     "--cw","8", "--check-cand-per-cl-max 100000",
     "--skip-cnf","--skip-stat-collect", "--cti-prune", 
     "--no-merge-cti",
     "--var-const-file varconst.txt"
  };


   vtg_cfg.FreqHornOptions = {
     "--ante-size", "1", "--conseq-size", "1", "--grammar pico.gmr",
     "--cw","8", "--check-cand-per-cl-max 100000",
     "--skip-cnf","--skip-stat-collect", "--cti-prune", 
     "--no-merge-cti",
     "--var-const-file varconst.txt"
   };



  vlg_cfg.pass_node_name = true;  // whether to use node name in Verilog

  std::string RootPath = "..";
  std::string VerilogPath = RootPath + "/verilog/";
  std::string RefrelPath = RootPath + "/refinement/";
  std::string OutputPath = RootPath + "/verification/Grain/";
  {
    os_portable_mkdir(OutputPath+"inv-enhance");
  }

  { // save grammar file
    std::string gmr = R"##(CTRL-STATE: m1.instr_bge, m1.is_alu_reg_reg, m1.irq_delay, m1.mem_state, m1.instr_beq, m1.instr_retirq, m1.instr_bgeu, m1.cpu_state, m1.instr_srli, m1.decoded_rs2, m1.irq_active, m1.instr_slt, m1.mem_do_prefetch, m1.instr_lbu, m1.is_beq_bne_blt_bge_bltu_bgeu, m1.instr_srai, m1.is_sll_srl_sra, m1.instr_sltu, m1.instr_slli, m1.instr_blt, m1.instr_jal, m1.instr_sltiu, m1.compressed_instr, m1.decoder_pseudo_trigger, m1.latched_store, m1.instr_sra, m1.instr_slti, m1.instr_bne, m1.is_sb_sh_sw, m1.instr_bltu
CTRL-OUT: m1.instr_bge, m1.is_alu_reg_reg, m1.instr_srli, m1.is_lb_lh_lw_lbu_lhu, m1.latched_store, m1.instr_retirq, m1.instr_bgeu, m1.irq_delay, m1.instr_lbu, m1.irq_active, m1.instr_slt, m1.mem_do_prefetch, m1.do_waitirq, m1.mem_do_rinst, m1.mem_valid, m1.is_beq_bne_blt_bge_bltu_bgeu, m1.mem_la_firstword_reg, m1.instr_srai, m1.is_sll_srl_sra, m1.instr_sltu, m1.instr_slli, m1.instr_blt, m1.prefetched_high_word, m1.instr_sltiu, m1.latched_branch, m1.compressed_instr, m1.decoder_pseudo_trigger, m1.instr_beq, m1.instr_sra, m1.instr_slti, m1.reg_sh, m1.instr_bne, m1.is_sb_sh_sw, m1.instr_bltu, m1.is_lui_auipc_jal, m1.instr_lui, m1.mem_do_rdata, m1.instr_rdinstrh
VAR-GROUP: m1.instr_slli, m1.instr_bge, m1.instr_sltu, m1.instr_srli, m1.instr_blt, m1.is_beq_bne_blt_bge_bltu_bgeu, m1.instr_slti, m1.instr_sltiu, m1.instr_beq, m1.is_sll_srl_sra, m1.instr_sra, m1.instr_slt, m1.is_alu_reg_reg, m1.instr_bne, m1.instr_bgeu, m1.is_sb_sh_sw, m1.instr_bltu
VAR-GROUP: m1.irq_delay, m1.irq_active
VAR-GROUP: m1.is_lb_lh_lw_lbu_lhu, m1.latched_branch, m1.latched_store
)##";

    std::ofstream fout(OutputPath + "inv-enhance/pico.gmr");
    if (fout.is_open())
      fout << gmr;
    else
      succeed = false;
  } // save grammar file


  { // save grammar file
    std::string gmr = R"##(1
m1.cpu_state 8 128 64 32 8 4 2 1
)##";

    std::ofstream fout(OutputPath + "inv-enhance/varconst.txt");
    if (fout.is_open())
      fout << gmr;
    else
      succeed = false;
  } // save grammar file

  InvariantSynthesizerCegar vg(
      {},                            // no include
      { VerilogPath + "picorv32.v"}, // designs

      "picorv32",                                             // top_module_name
      RefrelPath + "vmap.json",            // variable mapping
      RefrelPath + "cond.json",          // conditions of start/ready
      OutputPath,                                            // output path
      riscvILA.model.get(),                                           // model
      VerilogVerificationTargetGenerator::backend_selector::COSA, // backend: COSA
      VerilogVerificationTargetGenerator::synthesis_backend_selector::Z3, // synthesis backend: Z3
      vtg_cfg,  // target generator configuration
      vlg_cfg); // verilog generator configuration

  vg.LoadDesignSmtInfo(RootPath+"/smt/__design_smt.smt2");

  if (!succeed)
    return 3; 

  set_timeout(timeout, OutputPath, &n_cegar, &t_syn, & t_eq);

  do{
    vg.GenerateVerificationTarget();

    set_verify_timeout(10*60, OutputPath, &n_cegar, &t_syn, &t_eq);
    auto no_cex = vg.RunVerifAuto("ADD");
    cancel_verify_timeout(timeout, OutputPath, &n_cegar, &t_syn, &t_eq);

    vg.VerifGenCex(RootPath+"/cexs/"+std::to_string(n_cegar)+".vcd");

    if( no_cex ) {// the OPERATE 
      std::cerr << "No more Cex has been found! Cegar completes." << std::endl;
      break; // no more cex found
    }
    vg.ExtractVerificationResult();
    vg.CexGeneralizeRemoveStates({
      "m1.trace_data[35:0]", 
      "m1.timer[31:0]", 
      "m1.count_cycle[63:0]", 
      "m1.count_instr[63:0]",
      "m1.alu_out_q[31:0]", 
      "m1.mem_rdata_q[31:0]"});
    vg.GenerateSynthesisTarget();
    if(vg.RunSynAuto()) {
      std::cerr << "Cex is reachable! Cegar failed" << std::endl;
      succeed = false;
      break; // cex is really reachable!!!
    }
    vg.PrepareCexForGrain();
    { // FH
      InvariantInCnf cexs; //incremental_cnf
      //vg.ChangeFreqHornSyntax( fh_configs[cegar_idx] );
      //vg.ExtractInvariantVarForEnhance(vg.GetInvariants().NumInvariant() - 1, incremental_cnf, true,
      //   {"m1.trace_data", "m1.timer", "m1.count_cycle", "m1.count_instr", "m1.alu_out_q", "m1.mem_rdata_q",
      //    "m1.cpuregs","m1.reg_op2","m1.reg_op1", "m1.reg_pc" }
      //  );
      {
        std::ifstream fin(RootPath+"/cexs/"+std::to_string(n_cegar)+".cex");
        cexs.ImportFromFile(fin);
      }

      unsigned conseqsize = 1;
      unsigned nouse;
      bool use_grammar = true;
retry:
      //  vg.ChangeFreqHornSyntax(per_query_fh_config);
        if(!vg.WordLevelEnhancement(cexs)) {
          if (use_grammar) {
            std::cerr << "Failed on this iteration using grammar \n";
            auto newgrammar = base_fh_config;
            newgrammar[4] = "";
            vg.ChangeFreqHornSyntax(newgrammar);
            use_grammar = false;
            goto retry;
          } 
          std::cerr << "Failed on this iteration: increase conseq-size to : "<< (conseqsize+1) << "\n";

          //std::cerr << "press 1 enter to continue;\n";
          //std::cin >> nouse;

          auto newgrammar = base_fh_config;
          newgrammar[3] = std::to_string(++conseqsize);
          vg.ChangeFreqHornSyntax(newgrammar);
          if (conseqsize > 5) {
            std::cerr << "Failed even when conseq-size = " << (conseqsize-1) << "\n";
            succeed = false;
            break;
          } else
            goto retry;
        }
    }

    vg.ChangeFreqHornSyntax( base_fh_config );

    auto design_stat = vg.GetDesignStatistics();
    t_eq = design_stat.TimeOfEqCheck;
    t_syn = design_stat.TimeOfInvSynEnhance;
    t_total = design_stat.TimeOfEqCheck + design_stat.TimeOfInvSynEnhance;
    n_cegar ++;

  }while(not vg.in_bad_state());

  set_result(OutputPath, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);
  return 0;
}
