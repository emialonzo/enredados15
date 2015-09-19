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

#define IP_MULTICAST "225.5.4.48"
#define PUERTO_MULTICAST 8888

#define IP_CLIENTE "127.0.0.1"
#define PUERTO_CLIENTE 54321
#define USUARIO "usuario"

#define IP_SERVIDOR "127.0.0.1"
#define PUERTO_SERVIDOR 51234

#define MSGBUFSIZE 256

int main(int argc, char *argv[]) {

	cout << "Cant Argumentos: " << argc << endl;

	string usuario(USUARIO);

	string ip_multicast(IP_MULTICAST);
	int puerto_multicast(PUERTO_MULTICAST);

	string ip_cliente(IP_CLIENTE);
	int puerto_cliente(PUERTO_CLIENTE);

	string ip_servidor(IP_SERVIDOR);
	int puerto_servidor(PUERTO_SERVIDOR);

	if (argc == 2) {
		cout << "Argumentos: " << argv[0] << " - " << argv[1] << endl;
		usuario = argv[1];
	}
	if (argc == 3) {
		cout << "Argumentos: " << argv[0] << " - " << argv[1] << " - " << argv[2] << endl;
		usuario = argv[1];
		puerto_cliente = atoi(argv[2]);
	}
	cout << "Nombre Usuario: " << usuario << endl;
	cout << "IP Usuario: " << ip_cliente << endl;
	cout << "Puerto: " << puerto_cliente << endl;
	cout << "IP Multicast: " << ip_multicast << endl;
	cout << "Puerto Multicast: " << puerto_multicast << endl;

	//sock_multicast = socket(domain, type, protocol)
	// domain = AF_INET (IP address family)
	//type = SOCK_DGRAM (datagram service)
	//protocol = 0 (variaciones del protocolo)
	//Creo el socket para usar UDP y datagramas
	int socket_cliente = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_cliente < 0) {
		perror("No se pudo crear el socket");
		exit(0);
	}
	cout << "Se creo bien el socket" << endl;

	// defino la direccion del socket
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(puerto_cliente);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(socket_cliente,(struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		exit(1);
	}
	cout << "Se bindeo bien el socket" << endl;

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(ip_multicast.c_str());
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

  if (setsockopt(socket_cliente, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		perror("setsockopt");
		exit(1);
	}
	cout << "se setearon bien las opciones." << endl;

	socklen_t addrlen;
	int nbytes;
	char msgbuf[MSGBUFSIZE];

	while (1) {

		char buffer[MSGBUFSIZE];
	  bzero(buffer,MSGBUFSIZE);
	  fgets(buffer,MSGBUFSIZE,stdin);

		printf("este es el mensaje que le envio al servidor: %s\n",buffer);

		struct sockaddr_in addr_servidor;
	  memset((char *)&addr_servidor, 0, sizeof(addr_servidor));
	  addr_servidor.sin_family = AF_INET;
	  addr_servidor.sin_port = htons(puerto_servidor);
	  addr_servidor.sin_addr.s_addr = inet_addr(ip_servidor.c_str());

		int result = sendto(socket_cliente, buffer, strlen(buffer), 0, (struct sockaddr *)&addr_servidor, sizeof(addr_servidor));

	  if (result < 0) {
	    cout << "algo no salio bien cuando mandamos el mensaje." << endl;
	    perror("algo no salio bien cuando mandamos el mensaje.");
	    exit(1);
	  } else {
	    printf("se enviaron %d bytes\n", result);
	  }

		memset(&msgbuf, 0, MSGBUFSIZE);

		addrlen = sizeof(addr);
		if ((nbytes = recvfrom(socket_cliente, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		printf("Mensaje recibido desde el servidor: %s\n", msgbuf);
	}
	return 0;
}
