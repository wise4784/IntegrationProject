#
# This file is the CAN-fpga-app recipe.
#

SUMMARY = "Simple CAN-fpga-app application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://CAN-fpga-app.c \
	   file://Makefile \
		  "

S = "${WORKDIR}"

do_compile() {
	     oe_runmake
}

CLEANBROKEN = "1"

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 CAN-fpga-app ${D}${bindir}
}