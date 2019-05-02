cd D:/proj/42.Mutronics/LS_beam_ctrl/2.dev/fpga/LS_beam_ctrl/sources/proc_sys/
if { [ catch { xload xmp proc_sys.xmp } result ] } {
  exit 10
}
xset hdl vhdl
run stubgen
exit 0
