#
# This file is the ps-spi-app recipe.
#

SUMMARY = "Simple ps-spi-app application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://ps-spi-app.c \
	   file://Makefile \
		  "

S = "${WORKDIR}"

do_compile() {
	     oe_runmake
}

CLEANBROKEN = "1"

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 ps-spi-app ${D}${bindir}
}
