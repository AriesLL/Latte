package require control
package require struct::set

proc get_cell_type {cell_obj} {
  var type [get_property TYPE $cell_obj]
  return [regsub -all {\s+} $type {}]
}

proc ms_to_min {ms} {
  return [expr {$ms * .001 / 60}]
}


###############################################################################
## global variables ###########################################################
namespace eval GlobalVar {
  var number_of_cells     0
  var number_of_chains    0
  var number_of_nets      0
  var number_of_pins      0
  var number_of_sites     0

  array set cell_to_id    {}
  array set site_to_cells {}
  set site_types [list]

  var disable_write_cell  0
  var disable_write_chain 0
  var disable_write_net   0
  var disable_write_site  0
}
## end namespace GlobalVar



###############################################################################
## write cells ################################################################
namespace eval WriteCells {
var time_s [clock clicks -milliseconds]

var cells [get_cells -hier -filter {PRIMITIVE_LEVEL == LEAF && TYPE != Others}]
var cell_id 0

var fo_cells [open "$design_name.fnodes" "w"] 
var fo_place [open "$design_name.fpl" "w"]
foreach cell $cells {
  if ($GlobalVar::disable_write_cell) { break }
  var cell_type [get_cell_type $cell]
  var cell_site [get_property SITE $cell]
  var cell_bel [get_property BEL $cell]
  if {[string equal $cell_site ""] == 0} {
    set GlobalVar::cell_to_id($cell) $cell_id
    if {[info exists GlobalVar::site_to_cells($cell_site)]} {
      lappend GlobalVar::site_to_cells($cell_site) $cell
    } else {
      set GlobalVar::site_to_cells($cell_site) [list $cell]
    }
    var site_type [get_property SITE_TYPE $cell_site]
    ::struct::set include GlobalVar::site_types $site_type

    puts $fo_cells "$cell_id $cell_type $cell"
    puts $fo_place "$cell_id $cell_site $cell_bel"

    incr cell_id
  }
}
close $fo_place
close $fo_cells

set GlobalVar::number_of_cells $cell_id

var time_t [clock clicks -milliseconds]
puts [format {WriteCells time = %.2f min(s)} \
      [ms_to_min [expr $time_t - $time_s]]]
}
## end namespace WriteCells



###############################################################################
## write carry chains #########################################################
namespace eval WriteChains {
var time_s [clock clicks -milliseconds]

var fo [open "$design_name.fchains" "w"] 

var cells [get_cells -hier -filter {PRIMITIVE_LEVEL == LEAF && TYPE != Others}]
var num_tot_cells [llength $cells]

#report_carry_chains -file carry_chains.rpt -max_chains $num_tot_cells
var report \
  [report_carry_chains -return_string -max_chains $num_tot_cells]

var lines [split $report "\n"]
var num_lines [llength $lines]

var num_chains 0

  for {var li 0} {$li < $num_lines} {incr li} {
    var line [lindex $lines $li]
    if {[string first "| Carry Chains Count |" $line] == 0} {
      set li [expr $li + 2]
      var line [lindex $lines $li]
      var fields [regexp -all -inline {[^ |]+} $line]
      set num_chains [lindex $fields 0]
    } elseif {[string first "| Starting Instance" $line] == 0} {
      break
    }
  }
  set li [expr $li + 2]
  control::assert {$num_lines - $li + 1 >= $num_chains}

  var chain_id 0
  var chained_cell_id 0
  for {var k 0} {$k < $num_chains} {incr k} {
    if ($GlobalVar::disable_write_chain) { break }

    var line [lindex $lines [expr $li + $k]]
    var fields [regexp -all -inline {[^ |]+} $line]
    var chain_start [lindex $fields 0]
    var chain_length [lindex $fields 1]
    if {$chain_length <= 1} {continue}

    var chain_str ""
    var cell0 [get_cells $chain_start]
    var site0 [get_property SITE $cell0]
    var x0 0
    var y0 0
    scan $site0 {SLICE_X%dY%d} x0 y0
    for {var si 0} {$si < $chain_length} {incr si} {
      var yi [expr $y0 + $si]
      var site_i [format "SLICE_X%dY%d" $x0 $yi]
      var cell_i [get_cells $GlobalVar::site_to_cells($site_i) -filter {TYPE == CarryLogic}]
      control::assert {[llength $cell_i] == 1}
      set cell_i [lindex $cell_i 0]

      var cell_obj [get_cells $cell_i]
      var type [get_cell_type $cell_obj]
      var cell_id $GlobalVar::cell_to_id($cell_i)
      append chain_str "  $cell_id $type $cell_i\n"

      incr chained_cell_id
    }
    puts $fo "Chain: $chain_length"
    puts -nonewline $fo $chain_str
    incr chain_id
  }

  close $fo

  set GlobalVar::number_of_chains $chain_id

  var time_t [clock clicks -milliseconds]
  puts [format {WriteChains time = %.2f min(s)} \
        [ms_to_min [expr $time_t - $time_s]]]
}
## end namespace WriteChains



###############################################################################
## write nets #################################################################
namespace eval WriteNets {
var time_s [clock clicks -milliseconds]

var fo_nets [open "$design_name.fnets" "w"]
var nets [get_nets -hier]

# initialize global variable
set GlobalVar::number_of_nets 0
set GlobalVar::number_of_pins 0

foreach net $nets {
  if ($GlobalVar::disable_write_net) { break }
  var num_tot_pins [get_property PIN_COUNT $net]
  if {$num_tot_pins <= 1} {continue}
  var net_type [get_property TYPE $net]
  if {[string equal $net_type {SIGNAL}] == 0} {continue}

  var pins [get_pins -of_objects $net]
  var pins_str ""
  # count the number of effective pins
  var num_pins 0
  foreach p $pins {
    var c [get_cells -of_objects $p]
    var s [get_property SITE $c]
    if {[string equal $s ""] == 0} {
      var name [string range $p [expr [string length $c] + 1] [string length $p]]
      var dir [get_property DIRECTION $p]
      append pins_str "  $GlobalVar::cell_to_id($c) $name $dir\n"
      incr num_pins
    } else {
      # probably a tie-to-1 or tie-to-0 net
      set num_pins 0
      break
    }
  }
  if {$num_pins <= 1} { continue }

  # update global variable
  incr GlobalVar::number_of_pins $num_pins
  incr GlobalVar::number_of_nets

  var net_name [get_property NAME $net]
  puts $fo_nets "Net: $num_pins $net_name"
  puts -nonewline $fo_nets $pins_str
}

close $fo_nets

var time_t [clock clicks -milliseconds]
puts [format {WriteNetsWithPins time = %.2f min(s)} \
      [ms_to_min [expr $time_t - $time_s]]]
}
## end namespace WriteNets



###############################################################################
## write sites ################################################################
namespace eval WriteSites {
  var time_s [clock clicks -milliseconds]

  var fo [open "$design_name.fscl" "w"] 
  var site_id 0
  var tiles [get_tiles]
  foreach tile $tiles {
    if ($GlobalVar::disable_write_site) { break }
    var num_sites [get_property NUM_SITES $tile]
    if {$num_sites > 0} {
      var tile_row 0
      var tile_col 0 
      var tile_loc [string range $tile [string last _ $tile] [string length $tile]]
      scan $tile_loc {_X%dY%d} tile_row tile_col
      var site_list [get_sites -of_objects $tile]
      if {[llength $site_list] == 0} {continue}
      foreach site $site_list {
        var type [get_property SITE_TYPE $site]
        if {[::struct::set contains $GlobalVar::site_types $type] == 0} {continue}
        puts $fo "$site $type $tile_row $tile_col $tile"
        incr site_id
      }
    }
  }
  close $fo

  set GlobalVar::number_of_sites $site_id

  var time_t [clock clicks -milliseconds]
  puts [format {WriteSites time = %.2f min(s)} \
        [ms_to_min [expr $time_t - $time_s]]]
}
## end namespace WriteSites



###############################################################################
## write general ##############################################################
namespace eval WriteGeneral {
  var fo [open "$design_name.faux" "w"] 
  puts $fo "Design: ${design_name}"
  puts $fo "Device: [get_property PART [get_design]]"
  puts $fo "#cell: $GlobalVar::number_of_cells"
  puts $fo "#chain: $GlobalVar::number_of_chains"
  puts $fo "#net: $GlobalVar::number_of_nets"
  puts $fo "#pin: $GlobalVar::number_of_pins"
  puts $fo "#site: $GlobalVar::number_of_sites"
  puts $fo "site_types: $GlobalVar::site_types"
  close $fo
}
## end namespace WriteGereral
