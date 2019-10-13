#include <env.h>

#include <ilang/util/log.h>
#include <ilang/util/fs.h>
#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>

using namespace ilang;


int main (int argc, char ** argv) {
  SetLogLevel(2);

  std::string outDir = "../";
  // test z3
  {
    std::cout << "Test Z3: " << "(" << Z3PathFull  <<")" << std::endl;
    std::string prg = "z3";
    if (!std::string(Z3PathFull).empty())
      prg = os_portable_append_dir(Z3PathFull, prg);
    auto res = os_portable_execute_shell({prg,"-version"}, outDir + "z3-version.txt" , redirect_t::BOTH);
    std::cout << (res.subexit_normal ? "Okay" : "Error" )<< std::endl;
  }

  {
    std::cout << "Test CVC4: " << "(" << CVC4Path  <<")" << std::endl;
    std::string prg = "cvc4";
    if (!std::string(CVC4Path).empty())
      prg = os_portable_append_dir(CVC4Path, prg);
    auto res = os_portable_execute_shell({prg,"--version"},  outDir + "cvc4-version.txt" , redirect_t::BOTH);
    std::cout << (res.subexit_normal ? "Okay" : "Error") << std::endl;
  }


  {
    std::cout << "Test Grain: " << "(" << FREQHORNPath  <<")" << std::endl;
    std::string prg = "bv";
    if (!std::string(FREQHORNPath).empty())
      prg = os_portable_append_dir(FREQHORNPath, prg);
    auto res = os_portable_execute_shell({prg}, outDir + "grain-cfg.txt" , redirect_t::BOTH);
    std::cout << (res.subexit_normal ? "Okay" : "Error") << std::endl;
  }


  {
    std::cout << "Test ABC: " << "(" << ABCPath  <<")" << std::endl;
    std::string prg = "abc";
    if (!std::string(ABCPath).empty())
      prg = os_portable_append_dir(ABCPath, prg);
    auto res = os_portable_execute_shell({prg, "-c","help"}, outDir + "abc-cmds.txt" , redirect_t::BOTH);
    std::cout << (res.subexit_normal ? "Okay" : "Error") << std::endl;
  }

  {
    std::cout << "Test Yosys: " << "(" << YOSYSPath  <<")" << std::endl;
    std::string prg = "yosys";
    if (!std::string(YOSYSPath).empty())
      prg = os_portable_append_dir(YOSYSPath, prg);
    auto res = os_portable_execute_shell({prg, "-V"}, outDir + "yosys-version.txt" , redirect_t::BOTH);
    std::cout << (res.subexit_normal ? "Okay" : "Error") << std::endl;
  }

  {
    std::cout << "Test CoSA: " << "(" << COSAPath  <<")" << std::endl;
    std::string prg = "CoSA";
    std::string cwd = os_portable_getcwd();
    os_portable_chdir("../cosa-test/");
    auto res = os_portable_execute_shell({"bash", "run.sh"}, outDir + "cosa-run.txt" , redirect_t::BOTH);
    std::cout << (res.subexit_normal ? "Okay" : "Error") << std::endl;
    os_portable_chdir(cwd);
  }

  return 0;
}
