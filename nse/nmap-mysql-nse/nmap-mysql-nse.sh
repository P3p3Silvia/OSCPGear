#!/bin/bash
# NMAP NSE Scripts for mysql
# $1 is the first argument given after the bash script
# Check if argument was given, if not, print usage
file="./mysqlnse.txt"
if [ -z "$1" ]; then
echo "[*] Nmap-MYSQL NSE Script Scanner"
echo "[*] Usage : $0 <ip address> "
exit 0
fi
# if argument was given, scan that server with the nse
for server in $1;do
# For each of the lines on the text doc, attempt a nse scan
awk '{print $1}' < $file | while read nse; do nmap -p3306 --script $nse $server | tee -a ~/Desktop/$server-mysql-nse.txt; done
done







