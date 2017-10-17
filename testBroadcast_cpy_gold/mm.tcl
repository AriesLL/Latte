
############################
# Project settings
variable module [file rootname [info script]]
open_project .

add_files ${module}.cpp
add_files ${module}_test.cpp -tb
set_top ${module}
#############################
#Solution settings
delete_solution solution1
open_solution solution1
#
#set_part xc7vx485tffg1761-2
#set_part {xc7z020clg484-1}
set_part xc7vx690tffg1157-2
create_clock -period 10ns
set_clock_uncertainty 0

csim_design

#Synthesis
csynth_design
cosim_design -rtl verilog -trace_level all
export_design -format ip_catalog 
exit

