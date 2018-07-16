awk '{print $1}' < gobust.txt | while read ip; do gobuster -w /usr/share/wordlists/master-index.txt -u http://$ip -t 50 -s 200,204,301,302,307,403,401 > gobust-$ip && gobuster -w /usr/share/wordlists/master-index.txt -u http://$ip -t 50 -x html,php,cgi,asp,htm,com,jsp,txt,xml -s -s 200,204,301,302,307,403,401 >> gobust-$ip; done

