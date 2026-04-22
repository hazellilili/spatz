# Copyright 2021 ETH Zurich and University of Bologna.
# Solderpad Hardware License, Version 0.51, see LICENSE for details.
# SPDX-License-Identifier: SHL-0.51

onerror {resume}
quietly WaveActivateNextPane {} 0

proc getScriptDirectory {} {
    set dispScriptFile [file normalize [info script]]
    set scriptFolder [file dirname $dispScriptFile]
    return $scriptFolder
}

set scriptDir [getScriptDirectory]

# Add the cluster probe
add wave /tb_bin/i_dut/cluster_probe

# Add all cores
for {set core 0}  {$core < [examine -radix dec spatz_cluster_pkg::NumCores]} {incr core} {
    do ${scriptDir}/wave_core.tcl $core
}

# Add cluster waves
add wave -noupdate -group Cluster /tb_bin/i_dut/i_cluster_wrapper/i_cluster/*

add wave -group cluster_xbar /tb_bin/i_dut/i_cluster_wrapper/i_cluster/i_tcdm_interconnect/*

add wave -group ls_insn /tb_bin/i_dut/i_cluster_wrapper/i_cluster/gen_core[0]/i_spatz_cc/i_spatz/i_vlsu/spatz_mem_finished_o
add wave -group ls_insn /tb_bin/i_dut/i_cluster_wrapper/i_cluster/gen_core[0]/i_spatz_cc/i_spatz/i_vlsu/spatz_mem_str_finished_o
add wave -group ls_insn /tb_bin/i_dut/i_cluster_wrapper/i_cluster/gen_core[0]/i_spatz_cc/i_spatz/i_controller/spatz_req_o