#include "artifact_utility.h"
#include <env.h>

#include <ilang/util/fs.h>
#include <ilang/vtarget-out/vtarget_gen.h>
#include <ilang/vtarget-out/inv-syn/inv_syn_cegar.h>

using namespace ilang;

int main (int argc, char ** argv) {

  int timeout = get_timeout(argc, argv);

  int total_cegar = 7;

  int n_cegar = 0;
  double t_eq = 0;
  double t_syn= 0;
  double t_total = 0;
  bool succeed = true;

  std::string cwd = os_portable_getcwd();
  std::string verif_path = os_portable_append_dir(cwd, "../verification/PdrChc/");
  set_timeout(timeout, verif_path, &n_cegar, &t_syn, & t_eq);

  for (; n_cegar < total_cegar;  n_cegar ++) {
    if (!os_portable_chdir(verif_path) )
      std::cerr << "Failed to switch to " << verif_path << std::endl;
    std::string sub_path = verif_path + std::to_string(n_cegar);
    if (!os_portable_chdir(sub_path) )
      std::cerr << "Failed to switch to " << sub_path << std::endl;
    auto res = os_portable_execute_shell({"z3", "wrapper.smt2"});
    t_syn += res.seconds;
    t_total = t_eq + t_syn;
    std::cerr << n_cegar << std::endl;
  }

  set_result(verif_path, succeed,  t_syn + t_eq , n_cegar , t_syn , t_eq);

  return 0;
}
