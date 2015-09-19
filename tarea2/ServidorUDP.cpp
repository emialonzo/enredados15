#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

#define IP_SERVIDOR "127.0.0.1"
#define PUERTO_SERVIDOR 51234

#define IP_MULTICAST "225.5.4.48"
#define PUERTO_MULTICAST 8888

#define MSGBUFSIZE 256

int main (int argc, char *argv[]) {

  string ip_servidor(IP_SERVIDOR);
  int puerto_servidor(PUERTO_SERVIDOR);

  string ip_multicast(IP_MULTICAST);
  int puerto_multicast(PUERTO_MULTICAST);

  // Abro el socket UDP/IP
  int socket_servidor;
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

  socklen_t addrlen;
  int nbytes;
  char msgbuf[MSGBUFSIZE];

  while(1) {
    char buffer[MSGBUFSIZE];
    bzero(buffer,MSGBUFSIZE);
    //fgets(buffer,MSGBUFSIZE,stdin);

    memset(&msgbuf, 0, MSGBUFSIZE);
    addrlen = sizeof(addr);
    if ((nbytes = recvfrom(socket_servidor, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		printf("Mensaje recibido del cliente: %s\n", msgbuf);

    // char* mensajePrueba = "Este es un mensaje de prueba para mandar al multicast";
    // char * aux = (char *) mensajePrueba.c_str();

    strcat(buffer, "pepito: ");
    strcat(buffer, msgbuf);

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

  close(socket_servidor);
  cout << "Se cerro el socket" << endl;
}





//a
