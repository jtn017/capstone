############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project fll
set_top fll
add_files cmplx_fir.cpp
add_files cmplx_fir.h
add_files coef.h
add_files fll.cpp
add_files fll.h
add_files -tb tb_fll.cpp
open_solution "solution1" -flow_target vivado
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
#source "./fll/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
