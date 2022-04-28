############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project tec
set_top tec
add_files coefs.h
add_files tec.cpp
add_files tec.h
add_files -tb input.gold.dat
add_files -tb output.gold.dat
add_files -tb tb_tec.cpp
open_solution "solution1" -flow_target vivado
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
source "./tec/solution1/directives.tcl"
csim_design -clean
csynth_design
cosim_design
export_design -format ip_catalog
