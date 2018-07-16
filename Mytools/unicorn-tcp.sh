awk '{print $1}' < test-ip.txt | while read ip; do unicornscan -i tap0 -E $ip:a >>uniscan-tcp.txt; done
