#!/bin/bash
groupadd ftpgroup
useradd -g ftpgroup -d /dev/null -s /etc pepesilvia
pure-pw useradd pepesilvia -u pepesilvia -d /ftphome
pure-pw mkdb
cd /etc/pure-ftpd/auth/
ln -s ../conf/PureDB 60pdb
mkdir -p /ftphome
chown -R pepesilvia:ftpgroup /ftphome/
/etc/init.d/pure-ftpd restart
