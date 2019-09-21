set(VERSION "2.2.4")
set(MAINTAINER "Francesco Bertolaccini <francesco@bertolaccini.dev>")
set(COPYRIGHT "2019 - ${MAINTAINER}")
string(TIMESTAMP DPKG_DATE "%a, %d %b %Y %H:%M:%S +0000" UTC)

configure_file(debian/changelog.in debian/changelog @ONLY)
configure_file(debian/control.in debian/control @ONLY)
configure_file(debian/copyright.in debian/copyright @ONLY)