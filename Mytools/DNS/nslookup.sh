 #!/bin/bash

    for ip in $(seq 1 254); do
        nslookup 10.11.1.$ip 10.11.1.220 >> nslook-results.txt
    done
