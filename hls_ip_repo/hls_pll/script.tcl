############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project pll
add_files pll.cpp
add_files pll.h
add_files -tb input.gold.dat
add_files -tb output.gold.dat
add_files -tb pll_test.cpp
open_solution "solution1" -flow_target vivado
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
#source "./pll/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
