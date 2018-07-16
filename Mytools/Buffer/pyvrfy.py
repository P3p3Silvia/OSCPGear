import sys
import socket
users= []
for line in sys.stdin:
    line= line.strip()
    if line!='':
        users.append(line)

s= socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((sys.argv[1], 25))
fp= s.makefile('rwb')

fp.readline() # ignore banner
fp.write('HELO test.example.com\r\n')
fp.flush()
fp.readline() # ignore response

for user in users:
    fp.write('VRFY %s\r\n' % user)
    fp.flush()
    print '%s: %s' % (user, fp.readline().strip())

fp.write('QUIT\r\n')
fp.flush()
s.close()
