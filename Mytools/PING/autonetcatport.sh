awk '{print $1}' < labips.txt | while read ip; do nc -nvv -w 1 -z $ip >/dev/null 2>&1 && echo $ip IS UP || echo $ip IS DOWN; done
