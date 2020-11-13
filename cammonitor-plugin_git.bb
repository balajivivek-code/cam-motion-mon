iSUMMARY = "WebKitBrowser plugin"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"

PR = "r1"
PV = "1.0+git${SRCPV}"

SRC_URI = "git://github.com/balajivivek-code/cam-motion-mon.git;protocol=git;branch=main"

SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

H = "${WORKDIR}/build"

inherit cmake python3native

DEPENDS = "wpeframework wpeframework-tools-native"

EXTRA_OECMAKE += " \
    -DPYTHON_EXECUTABLE=${STAGING_BINDIR_NATIVE}/python3-native/python3 \
    -DCMAKE_SYSROOT=${STAGING_DIR_HOST} \
    -DBUILD_REFERENCE=${SRCREV} \
    -DBUILD_SHARED_LIBS=ON \
"

do_install () {

  install -d ${D}/usr/lib/wpeframework/plugins/
  
  install -d ${D}/etc/WPEFramework/plugins/  
      
  install -m 755 ${H}/*.so  ${D}/usr/lib/wpeframework/plugins/
 
  install -m 755 ${S}/*.json  ${D}/etc/WPEFramework/plugins/


}



FILES_SOLIBSDEV = ""
FILES_${PN} += "/usr/lib/wpeframework/plugins/"

INSANE_SKIP_${PN} += "libdir staticdev dev-so"
INSANE_SKIP_${PN}-dbg += "libdir"
