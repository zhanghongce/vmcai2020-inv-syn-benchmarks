#!/bin/bash
# cmake define vars
#define COSAEnvPath "/home/vmcai2020/cosaEnv/bin/activate"
#define COSAPath "/home/vmcai2020/CoSA/"
#define ABCPath "/home/vmcai2020/abc/"
#define YOSYSPath "/home/vmcai2020/yosys/"
#define FREQHORNPath "/home/vmcai2020/grain/"
#define CVC4Path "/home/vmcai2020/cvc4/"
#define Z3Path ""
# a
#

echo '-----build RC----------'
cd RC
bash buildAll.sh
mkdir -p verification/RelChc
mkdir -p verification/PdrChc
mkdir -p verification/PdrAbc
mkdir -p verification/Grain
mkdir -p verification/Cvc4Sy
cd ..
echo '-----build SP----------'
cd SP
bash buildAll.sh
mkdir -p verification/RelChc
mkdir -p verification/PdrChc
mkdir -p verification/PdrAbc
mkdir -p verification/Grain
mkdir -p verification/Cvc4Sy
cd ..
echo '-----build AES----------'
cd AES
bash buildAll.sh
mkdir -p verification/RelChc
mkdir -p verification/PdrChc
mkdir -p verification/PdrAbc
mkdir -p verification/Grain
mkdir -p verification/Cvc4Sy
cd ..
echo '-----build Pico----------'
cd Pico
bash buildAll.sh
mkdir -p verification/RelChc
mkdir -p verification/PdrChc
mkdir -p verification/PdrAbc
mkdir -p verification/Grain
mkdir -p verification/Cvc4Sy
cd ..
echo '-----build GB----------'
cd GB
bash buildAll.sh
mkdir -p verification/RelChc
mkdir -p verification/PdrChc
mkdir -p verification/PdrAbc
mkdir -p verification/Grain
mkdir -p verification/Cvc4Sy
cd ..


