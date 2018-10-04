#!/bin/bash
#variables
host=($1)
port=(80 25 443 445 21) 
s1=$(nmap -sS -oG - --open -p $port $host | grep -w Ports: | awk '{print $5}' | cut -d "/" -f1)
s2=($port)
#insert ip address
if [ -z "$1" ]; then
echo "[*] Nmap-FTP NSE Script Scanner"
echo "[*] Usage : $0 <ip address> "
exit 0
fi
#begin intial enumeration of ports
for port in 80 25 21 443 445
do
nmap -sS -oG - --open -p $port $host | grep -w Ports: | awk '{print $5}' | cut -d "/" -f1 >> /tmp/scantemp.txt
if [ "$s1" == "$s2" ]
then ./$port-nse.sh $host
else
    echo broke
fi
done
