#!/bin/bash

nmap -Pn --top-ports 1000 -sU --stats-every 3m --max-retries 1 -T3 -oA ~/Desktop/nmap-script/$1_U $1
