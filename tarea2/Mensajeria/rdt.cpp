
#include "rdt.h"
#include "constantes.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

//global vars
string usuario(USUARIO);

string ip_multicast(IP_MULTICAST);
int puerto_multicast(PUERTO_MULTICAST);

string ip_cliente(IP_CLIENTE);
int puerto_cliente(PUERTO_CLIENTE);

string ip_servidor(IP_SERVIDOR);
int puerto_servidor(PUERTO_SERVIDOR);
//fin global vars

void rdt_send(char* datos){
        cout << "Send RDT:::" << datos << ":::" << endl;
}

int socket_cliente = socket(AF_INET, SOCK_DGRAM, 0);
struct sockaddr_in addr;
struct ip_mreq mreq;
socklen_t addrlen;
int nbytes;
char msgbuf[MSGBUFSIZE];

void iniRdt(){
        if (socket_cliente < 0) {
                perror("No se pudo crear el socket");
                exit(0);
        }
        cout << "Se creo bien el socket" << endl;

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(puerto_cliente);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(socket_cliente,(struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("bind");
                exit(1);
        }
        cout << "Se bindeo bien el socket" << endl;

        mreq.imr_multiaddr.s_addr = inet_addr(ip_multicast.c_str());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);

        if (setsockopt(socket_cliente, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
                perror("setsockopt");
                exit(1);
        }
        cout << "se setearon bien las opciones." << endl;

}

char* recibir(){
        //char buffer[MSGBUFSIZE];
        // char* buffer = new char[MSGBUFSIZE];
        // bzero(buffer,MSGBUFSIZE);
        // fgets(buffer,MSGBUFSIZE,stdin);

        // struct sockaddr_in addr_servidor;
        // memset((char *)&addr_servidor, 0, sizeof(addr_servidor));
        // addr_servidor.sin_family = AF_INET;
        // addr_servidor.sin_port = htons(puerto_servidor);
        // addr_servidor.sin_addr.s_addr = inet_addr(ip_servidor.c_str());

        //int result = sendto(socket_cliente, buffer, strlen(buffer), 0, (struct sockaddr *)&addr_servidor, sizeof(addr_servidor));

        // if (result < 0) {
        //         perror("algo no salio bien cuando mandamos el mensaje.");
        //         exit(1);
        // } else {
        //         printf("se enviaron %d bytes\n", result);
        // }

        memset(&msgbuf, 0, MSGBUFSIZE);

        addrlen = sizeof(addr);
        if ((nbytes = recvfrom(socket_cliente, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
                perror("recvfrom");
                exit(1);
        }
        printf("Mensaje recibido desde el servidor: %s\n", msgbuf);
        char* retMen = new char[MSGBUFSIZE];
        strcpy(retMen, msgbuf);
        return retMen;
}


int socket_servidor;
void iniServer(){
  // Abro el socket UDP/IP

  if ((socket_servidor = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
    perror("No se pudo iniciar el socket");
    exit(1);
  }
  cout << "Se creo bien el socket" << endl;
  // Armo el struct para bindear la direccion y el puerto al socket
  struct sockaddr_in addr;
  memset((char *)&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(puerto_servidor);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(socket_servidor,(struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Hubo algun error en el bind");
    exit(1);
  }
  cout << "Se bindeo bien el socket" << endl;
}
void sendMulticast(char* mensaje){
  char buffer[MSGBUFSIZE];
  bzero(buffer,MSGBUFSIZE);
  strcat(buffer, mensaje);

  struct sockaddr_in addr_multicast;
  memset((char *)&addr_multicast, 0, sizeof(addr_multicast));
  addr_multicast.sin_family = AF_INET;
  addr_multicast.sin_port = htons(puerto_multicast);
  addr_multicast.sin_addr.s_addr = inet_addr(ip_multicast.c_str());

  int result = sendto(socket_servidor, buffer, strlen(buffer), 0, (struct sockaddr *)&addr_multicast, sizeof(addr_multicast));

  if (result < 0) {
    cout << "algo no salio bien cuando mandamos el mensaje." << endl;
    perror("algo no salio bien cuando mandamos el mensaje.");
    exit(1);
  } else {
    printf("se enviaron %d bytes\n", result);
  }
}
