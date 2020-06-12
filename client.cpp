#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

const bufferSize = 1024;
char message[bufferSize];
char buf[buffersize];
int PORT = 5000;
char * HOST = "127.0.0.1";

bool isNumber(const string s){
  return s.find_first_not_of( "0123456789-" ) == string::npos;
}

int main(int argc, char *argv[])
{
    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            if(strcmp(argv[i], "-p") == 0 && i + 1 < argc && isNumber(argv[i + 1])) {
                PORT = atoi(argv[i + 1]);
                i++;
            }
            if(strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
                HOST = argv[i + 1];
                i++;
            }
        }
    }

    int sock;
    struct sockaddr_in addr;
    cout << "Connecting to " << PORT << endl;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    printf(buf); 
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    inet_aton(HOST, &addr.sin_addr);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
    
    while(1) {  
        memset(&buf, 0, sizeof(buf));
        cout << "Введите запрос:\n";
        cin.getline(message, bufferSize);
        send(sock, message, sizeof(message), 0);
        recv(sock, buf, sizeof(message), 0);
        cout << buf << endl;
    }
    
    close(sock);

    return 0;
}