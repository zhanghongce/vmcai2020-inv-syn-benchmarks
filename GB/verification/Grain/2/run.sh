#!/bin/bash
/home/hongce/ila/aeval/build/tools/bv/bv --ante-size 0 --conseq-size 0 --cw 32 --skip-cnf --skip-stat-collect --skip-const-check --find-one-clause --cnf inv_grm wrapper.smt2 2> ../freqhorn.result
