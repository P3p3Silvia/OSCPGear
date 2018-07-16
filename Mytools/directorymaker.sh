awk '{print $1"-"$2}' < ~/Desktop/every-ipname.txt | while read ip; do mkdir $ip; done
