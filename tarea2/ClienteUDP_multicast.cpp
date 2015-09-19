#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <string>

using namespace std;

#define HELLO_PORT 8888
#define HELLO_GROUP "225.5.4.48"
#define MSGBUFSIZE 256

int main(int argc, char *argv[]) {
    string source_iface;
    string group(HELLO_GROUP);
    int port(HELLO_PORT);

    if (!(argc < 2)) group = argv[1];
    if (!(argc < 3)) port = atoi(argv[2]);
    if (!(argc < 4)) source_iface = argv[3];

    cout << "group: " << group << " port: " << port << " source_iface: " << source_iface << endl;

    int fd;
    //AF_INET indica que es un socket de red, podría ser AF_UNIX para indicar que sea un proceso de la misma maq
    //SOCK_DGRAM indica UDP
    // 0 indica "el protocolo" no tengo idea
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    u_int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (yes)) < 0) {
        perror("Reusing ADDR failed");
        exit(1);
    }


    //estructura del socket de entrada
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //s_addr es la dirección IP del cliente al que queremos atender
//    addr.sin_addr.s_addr = (group.empty() ? htonl(INADDR_ANY) : inet_addr(group.c_str()));
    addr.sin_addr.s_addr =  htonl(INADDR_ANY) ;

    if (bind(fd, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
        perror("bind");
        exit(1);
    }

    struct ip_mreq mreq;
    //struct in_addr imr_multiaddr;   /* IP multicast address of group */
    //struct in_addr imr_interface;   /* local IP address of interface */
    mreq.imr_multiaddr.s_addr = inet_addr(group.c_str()); //ip de multicast
    mreq.imr_interface.s_addr = (source_iface.empty() ? htonl(INADDR_ANY) : inet_addr(source_iface.c_str())); 

    //IPPROTO_IP indica que va a correr sobre la capa ip (podria ser socket)
    
    // IP_MULTICAST_LOOP -- indica si los datos seran devueltos al host o no 
    //u_char loop;
    //setsockopt(socket, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
    
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof (mreq)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    socklen_t addrlen;
    int nbytes;
    char msgbuf[MSGBUFSIZE];

    while (1) {
        memset(&msgbuf, 0, MSGBUFSIZE);

        addrlen = sizeof (addr);
        if ((nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *) &addr, &addrlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        cout.write(msgbuf, nbytes);
        cout.flush();
    }
    return 0;
}