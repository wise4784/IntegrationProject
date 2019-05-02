proc exportToSDK {} {
  cd D:/proj/42.Mutronics/LS_beam_ctrl/2.dev/fpga/state1/LS_beam_ctrl/sources/proc_sys
  if { [ catch { xload xmp proc_sys.xmp } result ] } {
    exit 10
  }
  if { [run exporttosdk] != 0 } {
    return -1
  }
  return 0
}

if { [catch {exportToSDK} result] } {
  exit -1
}

exit $result
