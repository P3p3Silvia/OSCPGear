awk '{print $1}' < smb-open-port-boxes.txt | while read ip; do nmap -p445 --script smb-vuln-ms17-010 $ip >> eternalblue.txt; done
