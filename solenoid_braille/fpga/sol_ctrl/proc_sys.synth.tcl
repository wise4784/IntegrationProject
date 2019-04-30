proc pnsynth {} {
  cd C:/sol_ctrl_0429/sol_ctrl/source/proc_sys
  if { [ catch { xload xmp proc_sys.xmp } result ] } {
    exit 10
  }
  if { [catch {run netlist} result] } {
    return -1
  }
  return $result
}
if { [catch {pnsynth} result] } {
  exit -1
}
exit $result
