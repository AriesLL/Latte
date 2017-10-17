##vivado.tcl
#add source

open_project /curr/peipei/PhD/EnergyPipeline/0benchSuite/FPGA16/testBroadcast_sweep/testBroadcast_cpy/solution1/impl/ip/tmp.xpr
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

add_files -norecurse -scan_for_includes /curr/peipei/PhD/EnergyPipeline/0benchSuite/FPGA16/testBroadcast_sweep/testBroadcast_cpy/solution1/impl/verilog
import_files -norecurse /curr/peipei/PhD/EnergyPipeline/0benchSuite/FPGA16/testBroadcast_sweep/testBroadcast_cpy/solution1/impl/verilog
update_compile_order -fileset sources_1
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1
update_compile_order -fileset sources_1

#add xdc
add_files -fileset constrs_1 -norecurse /curr/peipei/PhD/EnergyPipeline/0benchSuite/FPGA16/testBroadcast_sweep/testBroadcast_cpy/solution1/impl/verilog/mm.xdc
import_files -fileset constrs_1 /curr/peipei/PhD/EnergyPipeline/0benchSuite/FPGA16/testBroadcast_sweep/testBroadcast_cpy/solution1/impl/verilog/mm.xdc

#add sim_files
set_property SOURCE_SET sources_1 [get_filesets sim_1]

add_files -fileset sim_1 -norecurse -scan_for_includes /curr/peipei/PhD/EnergyPipeline/0benchSuite/FPGA16/testBroadcast_sweep/testBroadcast_cpy/solution1/sim/verilog
import_files -fileset sim_1 -norecurse /curr/peipei/PhD/EnergyPipeline/0benchSuite/FPGA16/testBroadcast_sweep/testBroadcast_cpy/solution1/sim/verilog

update_compile_order -fileset sim_1
update_compile_order -fileset sim_1

set_property top apatb_mm_top [get_filesets sim_1]
set_property top_lib xil_defaultlib [get_filesets sim_1]
update_compile_order -fileset sim_1

#setting sim_settings
set_property -name {xsim.simulate.runtime} -value {140000000000ns} -objects [current_fileset -simset]
set_property -name {xsim.simulate.uut} -value {*} -objects [current_fileset -simset]
set_property -name {xsim.simulate.saif} -value {testFlow.saif} -objects [current_fileset -simset]
set_property -name {xsim.simulate.saif_all_signals} -value {true} -objects [current_fileset -simset]

#launch_simulation
launch_simulation
#lauch implementation
launch_runs synth_1
wait_on_run synth_1
if {[get_property PROGRESS [get_runs synth_1]] != "100%"} {
error "ERROR: synth_1 failed"
}
launch_runs impl_1
wait_on_run impl_1


open_run impl_1


set_load 0 [all_outputs]
read_saif {/curr/peipei/PhD/EnergyPipeline/0benchSuite/FPGA16/testBroadcast_sweep/testBroadcast_cpy/solution1/impl/ip/tmp.sim/sim_1/behav/testFlow.saif}
report_power -hier all -l 10000000 -verbose -file {/curr/peipei/xilinx/testFlow.pwr} -xpe {/curr/peipei/xilinx/testFlow.xpe} -name {testFlow}
#%29 nets 

set_property -name {xsim.simulate.saif} -value {testFlow_impl.saif} -objects [current_fileset -simset]
launch_simulation -mode post-implementation -type timing


#source apatb_mm_top.tcl
set_load 0 [all_outputs]
read_saif {/curr/peipei/PhD/EnergyPipeline/0benchSuite/FPGA16/testBroadcast_sweep/testBroadcast_cpy/solution1/impl/ip/tmp.sim/sim_1/impl/timing/testFlow_impl.saif}
report_power -hier all -l 10000000 -verbose -file {/curr/peipei/xilinx/testFlow_impl.pwr} -xpe {/curr/peipei/xilinx/testFlow_impl.xpe} -name {testFlow_impl}

#54% of nets annotated
