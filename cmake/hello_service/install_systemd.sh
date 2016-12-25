#!/bin/sh

LIB_SYSTEMD_SYSTEM_PATH="/etc/systemd/system"
SBIN_PATH="/usr/bdk/sbin"
mkdir -p ${SBIN_PATH}

cp -f systemd/hello ${SBIN_PATH}
chmod 755 ${SBIN_PATH}/hello
chown root.root ${SBIN_PATH}/hello

cp -f systemd/hello.service ${LIB_SYSTEMD_SYSTEM_PATH}
chmod 644 ${LIB_SYSTEMD_SYSTEM_PATH}/hello.service
chown root.root ${LIB_SYSTEMD_SYSTEM_PATH}/hello.service

# enable/start hello as a systemd service.
#systemctl enable hello
#systemctl start hello
