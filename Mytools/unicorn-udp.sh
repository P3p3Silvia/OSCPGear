awk '{print $1}' < every-lab-ip.txt | while read ip; do unicornscan -mU $ip >>uniscan-udp.txt; done
