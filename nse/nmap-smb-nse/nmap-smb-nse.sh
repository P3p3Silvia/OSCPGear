#!/bin/bash
# NMAP NSE Scripts for SMB
# $1 is the first argument given after the bash script
# Check if argument was given, if not, print usage
file="./nmapsmbnse.txt"
if [ -z "$1" ]; then
echo "[*] Nmap-SMB NSE Script Scanner"
echo "[*] Usage : $0 <ip address> "
exit 0
fi
# if argument was given, scan that server with the nse
for server in $1;do
# For each of the lines on the text doc, attempt a nse scan
awk '{print $1}' < $file | while read nse; do nmap -p445 --script $nse $1 $server | tee -a ~/Desktop/$server-smb-nse.txt; done
done







