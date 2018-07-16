awk '{print $1}' < snmp-open-ip.txt | while read ip; do snmpwalk -c public -v1 $ip >> snmpwalkresult.txt; done
