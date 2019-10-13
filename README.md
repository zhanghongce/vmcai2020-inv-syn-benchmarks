
Structure of the Test Cases
=====================

  * This test case folder contains five examples as mentioned in the paper
    - `RC` : redundant counters
    - `SP` : a simple processing pipeline
    - `AES` : the AES block encryption accelerator
    - `Pico` : PicoRV32 processor
    - `GB` : the Gaussian-blur image processing accelerator
    - `configs` : CMake configuration for the project
    - `test` : a self-test utility to check if installation is good

  In each folders of the five examples, the files are orginized in the following way:

  * `cmake` : for CMake scripts
  * `include` : contains headers for time-counting utilities and headers for the ILA models of each design
  * `src` : source for time-counting utilities and the ILA model
  * `rfmap` or `refinement` : the refinement map, in JSON format
  * `verilog` : the Verilog implementation of the designs
  * `grm` : the grammar used by Grain
  * `app` : the functional equivalence checking and invariant synthesis procedure

  In the `app` subfolder, there are five source files in the following format:
  ```
  main-xxxx.cc
  ```
  Where `xxxx` stands for the five methods we evaluate: `pdrchc`, `pdrabc`, `relchc`, `cvc4sy` and `grain`.


To Use the Test Cases
=====================

Please refer to [this Github page](https://github.com/zhanghongce/VMCAI-2020-AE)