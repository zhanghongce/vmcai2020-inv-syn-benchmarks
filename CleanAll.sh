
rm -f configs/local.cfg
echo '-----clean RC----------'
cd RC
rm -f include/env.h
rm -rf verification/RelChc/*
rm -rf verification/PdrChc/*
rm -rf verification/PdrAbc/*
rm -rf verification/Grain/*
rm -rf verification/Cvc4Sy/*
cd ..
echo '-----clean SP----------'
cd SP
rm -f include/env.h
rm -rf verification/RelChc/*
rm -rf verification/PdrChc/*
rm -rf verification/PdrAbc/*
rm -rf verification/Grain/*
rm -rf verification/Cvc4Sy/*
cd ..
echo '-----clean AES----------'
cd AES
rm -f include/env.h
rm -rf verification/RelChc/*
rm -rf verification/PdrChc/*
rm -rf verification/PdrAbc/*
rm -rf verification/Grain/*
rm -rf verification/Cvc4Sy/*
cd ..
echo '-----clean Pico----------'
cd Pico
rm -f include/env.h
rm -rf  verification/RelChc/*
rm -rf  verification/PdrChc/*
rm -rf  verification/PdrAbc/*
rm -rf  verification/Grain/*
rm -rf  verification/Cvc4Sy/*
cd ..
echo '-----clean GB----------'
cd GB
rm -f include/env.h
rm -rf  verification/RelChc/result-stat.txt
rm -rf  verification/PdrChc/result-stat.txt
rm -rf  verification/PdrAbc/result-stat.txt
rm -rf  verification/Grain/result-stat.txt
rm -rf  verification/Cvc4Sy/result-stat.txt
cd ..



