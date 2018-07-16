#!/usr/bin/python

chars = ''
for i in range(0x30, 0x35):
	for j in range(0x30, 0x3A):
		for k in range(0x30, 0x3A):
			chars += chr(i) + chr(j) + chr(k) + 'A'
print chars
