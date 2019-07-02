## NNDescent

Pizza Team C implementation of NNDescent to use for XOHW19

#### Python Code Generation

Commands useful to use the code:

1. modify **main.py** in order to fit your needs

2. launch 

   ```
   python main.py
   ```

   in order to generate the correct C implementation of nn_descent

3. Start Vivado Synthesis in order to generate the bitstream

#### Vivado HLS 2018.2 ####
- New Project (PYNQ Part: xc7z020clg400-1)

- Source/Test:

  - include Source Files

    ```
    /hls/[defines.h, headers.h, heap_utils.c, nn_descent.c, utils.c]
    ```

  - include as Test Files

    ```
    /hls/[main.c, signals.h]
    ```

- Select **nn_descent** as Top function

- Simulation

- Synthesis (set target time (eg. 10ns == 100Mhz)

- Cosimulation

- Solution -> Export RTL


#### Vivado 2018.2  ####
- New Project (PYNQ Part: xc7z020clg400-1)
- Create Block Design
- Import Cores:
  - IP setting (right clicking the white space in block design)
  - Repository Management
  - Add Cores by selecting HLS directory (if "no IPs Interfaces" => forgot "Export RTL" in HLS)

=========================== [Board PYNQ]

* download: Zynq Presets https://reference.digilentinc.com/reference/programmable-logic/pynq-z1/star

  (or use the pynq_revC.tcl provided in the repository)

* add: ZYNQ7 Processing System

  * double click -> add preset ...
  *  PS-PL Configuration -> Axi non secure -> GP Master -> Enable GP0 (to communicate through axi lite)
  * PS-PL Configuration -> HP Slave Axi -> Enable HP0, HP1, HP2, HP3
  * Autoconnect

* add: **nn_descent Core**

===========================

- Validate Design
- Sources -> Design (dx click) -> create HDL Wrapper [needed to create drivers interfaces]
- Generate Bitstream
- File -> Export -> Hardware -> Include Bitstream
- File -> Launch SDK


#### SDK 2016.1 (Windows) ####
- File -> New Application Proj -> (check correct .hdf (if it is to import select Target HW -> New -> Browse.. and Pray)) -> [...] (Helloworld)
- copy-paste sdk/[helloworld.c, signals.h, defines.h] in the src directory
- Src -> Iscript -> (augment Heap and Stack: 0x1f000000 or less if error)
- Connect PYNQ
- Program FPGA -> Program
- Run Configurations..  -> Xilinx Application (GDB)
  - Target Setup (Bitstream file: [select] / Initialization file: [select] / Reset Processor: select / Program FPGA: activate / Run ps7_init: activate / Run ps7_post_config: activate)
  - Application (select application and Proj name)
  -  sudo screen /dev/ttyUSB2 115200 (to print into terminal the output of selected port eg. ttyUSB2)
- Run
