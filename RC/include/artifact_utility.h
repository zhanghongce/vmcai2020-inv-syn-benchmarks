#ifndef ARTIFACT_UTILITY_H__
#define ARTIFACT_UTILITY_H__

#include <string>     // std::string, std::stoi

int get_timeout(int argc, char ** argv);
int set_result(const std::string & outDir, bool succeeded, double total_sec, int cegar_iter, double syn_time, double eq_time);
int set_timeout(int sec);

#endif // ARTIFACT_UTILITY_H__
