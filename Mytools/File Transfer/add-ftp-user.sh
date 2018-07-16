#!/bin/bash
groupadd ftpgroup
useradd -g ftpgroup -d /dev/null -s /etc ftpguy
pure-pw useradd ftpguy -u ftpguy -d /ftphome
pure-pw mkdb
cd /etc/pure-ftpd/auth/
ln -s ../conf/PureDB 60pdb
mkdir -p /ftphome
chown -R ftpguy:ftpgroup /ftphome/
/etc/init.d/pure-ftpd restart
