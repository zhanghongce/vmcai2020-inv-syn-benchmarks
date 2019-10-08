#include "artifact_utility.h"

#include <fstream>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

int get_timeout(int argc, char ** argv) {
  int timeout;
  if (argc <= 1)
    return 3600*10;
  else {
    try {
      timeout = stoi(argv[1]);
    } catch (...) {
      timeout=3600*10;
    }
  }
  return timeout;
}

void set_result(const std::string & outDir, bool succeeded, double total_sec, int cegar_iter, double syn_time, double eq_time) {
  std::ofstream fout (outDir + "result-stat.txt");
  if (succeeded)
    fout << "DONE\n";
  else
    fout << "FAIL\n";
  fout << total_sec << std::endl;
  fout << cegar_iter<< std::endl; // iter
  fout << syn_time << std::endl; // syn
  fout << eq_time << std::endl; // eqcheck
}

volatile sig_atomic_t int * glb_cegar_iter = NULL;
volatile sig_atomic_t double * glb_syn_time = NULL;
volatile sig_atomic_t double * glb_eq_time = NULL;
volatile sig_atomic_t std::string glb_outDir;
 
void handle_alarm( int sig ) {
  int cegar_iter    = glb_cegar_iter ? * glb_cegar_iter : 0;
  double syn_time   = glb_syn_time ? * glb_syn_time : 0;
  double eq_time    = glb_eq_time ? * glb_eq_time : 0;
  set_result(glb_outDir, false, syn_time + eq_time, cegar_iter, syn_time, eq_time);

  kill(0, SIGTERM);
  exit(0);
}

void set_timeout(int sec,  const std::string & outDir,   int * cegar_iter, double * syn_time, double * eq_time) {
  glb_cegar_iter = cegar_iter;
  glb_syn_time   = syn_time;
  glb_eq_time    = eq_time;
  glb_outDir     = outDir;


  signal( SIGALRM, handle_alarm );
  alarm (sec);

}
