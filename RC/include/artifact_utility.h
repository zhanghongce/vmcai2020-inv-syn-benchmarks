#ifndef ARTIFACT_UTILITY_H__
#define ARTIFACT_UTILITY_H__

#include <string>     // std::string, std::stoi

int get_timeout(int argc, char ** argv);
void set_result(const std::string & outDir, bool succeeded, double total_sec, int cegar_iter, double syn_time, double eq_time);
void set_timeout(int sec,  const std::string & outDir,   int * cegar_iter, double * syn_time, double * eq_time);

#endif // ARTIFACT_UTILITY_H__
