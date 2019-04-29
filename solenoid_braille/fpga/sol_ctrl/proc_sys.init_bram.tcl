cd C:/Users/koitt01A/Desktop/fan_ctrl_0430/fan_ctrl/source/proc_sys
if { [ catch { xload xmp proc_sys.xmp } result ] } {
  exit 10
}

if { [catch {run init_bram} result] } {
  exit -1
}

exit 0
