############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project data2ps
set_top data2ps
add_files data2ps.h
add_files data2ps.cpp
open_solution "solution1" -flow_target vivado
set_part {xc7z010i-clg225-1L}
create_clock -period 10 -name default
config_export -format ip_catalog -rtl verilog
source "./data2ps/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -rtl verilog -format ip_catalog
