awk '{print $1}' < smb-win-ip.txt | while read ip; do enum4linux -a $ip >> enum4-win.txt; done
