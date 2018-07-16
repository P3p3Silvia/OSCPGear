#!/bin/bash

nmap -nvv -Pn -sSV -T1 -p$(cat ~/Desktop/nmap-script/$1_T.xml | grep portid | grep protocol=\"tcp\" | cut -d'"' -f4 | paste -sd "," -) --version-intensity 9 -A -oA ~/Desktop/nmap-script/$1_T_DETAILED $1
