#ifndef RC_ILA_H__
#define RC_ILA_H__

#include <ilang/ilang++.h>

Ila BuildModel();

// artifect utility
int get_timeout(int argc, char ** argv);
void set_result(const char * outDir, bool succeeded, double total_sec, int cegar_iter, double syn_time, double eq_time);
void set_timeout(int sec);

#endif // RC_ILA_H__
