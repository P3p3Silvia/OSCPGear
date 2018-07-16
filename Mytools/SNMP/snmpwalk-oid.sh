awk '{print $1}' < snmpwinboxip.txt | while read ip; do snmpwalk -c public -v1 $ip 1.3.6.1.4.1.77.1.2.25 && snmpwalk -c public -v1 $ip 1.3.6.1.2.1.25.4.2.1.2; done
