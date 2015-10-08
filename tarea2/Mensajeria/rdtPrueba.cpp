
#include "rdtPrueba.h"
#include "constantes.h"
#include "mensaje.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <map>

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

int socket_servidor;

int soc_ser;


void rdt_send(char* datos){
  cout << "Send RDT:::" << datos << ":::" << endl;
}

//mapa con clave ip y numero de secuancia
typedef map<char*, int> TablaSecuencias;
// typedef struct {
TablaSecuencias* emisor = new TablaSecuencias;
TablaSecuencias* receptor = new TablaSecuencias;
// }Secuencias;
//
// Secuencias* s = new Secuencias;


// • rdt_send(datos), recibe datos desde la capa superior.
// • udt_send_broadcast(pkt), le entrega el paquete pkt a la capa inferior para ser transmitido a todos los
// computadores de la red.
// • rdt_rcv(pkt), recibe los paquetes desde la capa inferior.
// • deliver_data(datos), entrega datos a la capa superior.
// • make_pkt(numeroDeSecuencia, datos, checksum, origen)
// • extract_data(pkt, datos), extrae los datos de un paquete.
// • extract_orig(pkt, ip), extrae la dirección de origen del paquete.
// • corrupt(pkt), verdadero sii pkt fue corrompido durante su transmisión.
// • isACK(pkt, numeroDeSecuencia), verdadero sii pkt es un ACK con número de secuencia numeroDeSecuencia


int CrearSocket(int puerto, bool multicast){
  struct sockaddr_in addr;
  struct ip_mreq mreq;
  int sockete = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockete < 0) {
    perror("No se pudo crear el socket");
    exit(0);
  }
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(puerto);
  //TODO atender unicamente ip servidor e ip multicast
  addr.sin_addr.s_addr = htonl(INADDR_ANY); //direccion que atiende

  if (bind(sockete, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }
  //seteando valores del socket multicast
  if(multicast){
    mreq.imr_multiaddr.s_addr = inet_addr(ip_multicast.c_str());
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    //IPPROTO_IP indica que va a correr sobre la capa ip (podria ser socket)
    // IP_MULTICAST_LOOP -- indica si los datos seran devueltos al host o no
    if (setsockopt(sockete, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
      perror("setsockopt");
      exit(1);
    }
  }
  return sockete;
}

int getSequenceNumber(TablaSecuencias* tabla, char* ipFrom){
  TablaSecuencias::iterator it = tabla->find(ipFrom);
  if(it == receptor->end()){
    char* ip = new char[strlen(ipFrom)];
    strcpy(ip, ipFrom);
    receptor->insert(make_pair(ip,0));
    it = receptor->find(ip);
    // (*receptor)[ipFrom]=0;
  }
  return it->second;
}

void updateSequenceNumber(TablaSecuencias* tabla, char* ip, int num){
  (*tabla)[ip]=num;
}

char* rdt_recibe(int soc){
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  socklen_t addrlen;

  int nbytes;

  //char msgbuf[MSGBUFSIZE];
  rdtMsj* mensaje;

  // bool mensajeOk=false;
  while(true){

    memset(&mensaje , 0, sizeof(mensaje));

    addrlen  = sizeof(addr);
    //recibimos UDP
    if ((nbytes = recvfrom(soc, (char*) mensaje, sizeof(mensaje), 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
      perror("recvfrom");
      exit(1);
    }

    char* ipFrom = inet_ntoa(addr.sin_addr);
    //s->receptor->find(ipFrom);

    // TablaSecuencias::iterator it = receptor->find(ipFrom);
    // if(it == receptor->end()){
    //   char* ip = new char[strlen(ipFrom)];
    //   strcpy(ip, ipFrom);
    //   receptor->insert(make_pair(ip,0));
    //   it = receptor->find(ip);
    //   // (*receptor)[ipFrom]=0;
    // }
    // int seqEsperado = it->second;
    int seqEsperado = getSequenceNumber(receptor, ipFrom);

    rdtMsj* respuesta = new rdtMsj;
    respuesta->esAck=true;
    // respuesta.from = "importa?";
    // respuesta.mensaje="importa munia?";
    respuesta->esMulticast=mensaje->esMulticast;
    //envio ACK ok
    respuesta->seq=seqEsperado;

    int result = sendto(soc, respuesta, sizeof(respuesta), 0, (struct sockaddr *)&addr, addrlen);
    if( (result>0) && (seqEsperado == mensaje->seq)){ //(&& result > 0 )
      //actualizo tabla
      int newSeq = (++seqEsperado)%2;
      // it->second = newSeq;
      updateSequenceNumber(receptor, ipFrom, newSeq);
      //todo ok :)
      printf("DEBUG::Mensaje recibido: %s\n", mensaje->mensaje);
      char* retMen = new char[MSGBUFSIZE];
      strcpy(retMen, mensaje->mensaje);

      return retMen;
    }

  }

}

void rdt_sendto(int soc, char* mensajeToSend, char* ip, int puerto){

  int nbytes;
  socklen_t addrlen;
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(puerto);
  addr.sin_addr.s_addr = inet_addr(ip);

  int seqEsperado = getSequenceNumber(emisor, ip);

  while(true){
    addrlen  = sizeof(addr);
    // int result = sendto(soc, respuesta, sizeof(respuesta), 0, (struct sockaddr *)&addr, addrlen);

    rdtMsj* mensaje;
    memset(&mensaje , 0, sizeof(mensaje));
    mensaje->esAck=false;
    strcpy(mensaje->from, "esNecesario?");
    strcpy(mensaje->mensaje, mensajeToSend);
    mensaje->esMulticast=false;
    mensaje->seq = getSequenceNumber(emisor, ip);

    int result = sendto(soc, (char*) mensaje, sizeof(*mensaje), 0, (struct sockaddr *)&addr, addrlen);

    if(result >0){
      rdtMsj* mensajeRcb;
      memset(&mensajeRcb , 0, sizeof(mensaje));
      //envio ACK ok
      if ((nbytes = recvfrom(soc, (char*) mensajeRcb, sizeof(*mensajeRcb), 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
        perror("recvfrom");
        exit(1);
      }

      char* ipFrom = inet_ntoa(addr.sin_addr);
      if( (strcmp(ipFrom, ip)==0) && (mensajeRcb->esAck) &&  (mensajeRcb->seq==seqEsperado)){
        //mismo ip
        updateSequenceNumber(emisor, ip, (++seqEsperado)%2);
        printf("DEBUG::Mensaje enviado: %s\n", mensaje->mensaje);
        return ;
      }
      else{
        printf("DEBUG::Error: :(\n");
      }
    }

  }

}

//TODO cambiar nombre de socket a socket multicast
// void recibeServidorIni(){
//   //creamos socket UDP
//   soc_ser = socket(AF_INET, SOCK_DGRAM, 0);
//   if (socket_cliente < 0) {
//     perror("No se pudo crear el socket");
//     exit(0);
//   }
//   cout << "Se creo bien el socket" << endl;
//
//   //se setean valores del socket, puerto, ip
//   memset(&addr, 0, sizeof(addr));
//   addr.sin_family = AF_INET;
//   addr.sin_port = htons(puerto_servidor);
//   //TODO atender unicamente ip servidor e ip multicast
//   addr.sin_addr.s_addr = htonl(INADDR_ANY); //direccion que atiende
//
//   if (bind(socket_cliente,(struct sockaddr *)&addr, sizeof(addr)) < 0) {
//     perror("bind");
//     exit(1);
//   }
// }
int multicastSeq=0;
void rdt_send_multicast(int soc, char* mensajeToSend, TablaClienteId* tablaClientes){
  //estructuras
  struct sockaddr_in addr;
  int nbytes;
  socklen_t addrlen;

  //armo direccion del multicast
  struct sockaddr_in addr_multicast;
  memset((char *)&addr_multicast, 0, sizeof(addr_multicast));
  addr_multicast.sin_family = AF_INET;
  addr_multicast.sin_port = htons(puerto_multicast);
  addr_multicast.sin_addr.s_addr = inet_addr(ip_multicast.c_str());

  //armo mensaje a enviar
  rdtMsj* mensaje = new rdtMsj;
  memset(&mensaje , 0, sizeof(mensaje));
  mensaje->esAck=false;
  strcpy(mensaje->from, "esNecesario?");
  strcpy(mensaje->mensaje, mensajeToSend);
  mensaje->esMulticast=true;
  mensaje->seq = multicastSeq;
  while(true){
    //envio mensaje a multicast
    int result = sendto(socket_servidor, mensaje, sizeof(mensaje), 0, (struct sockaddr *)&addr_multicast, sizeof(addr_multicast));

    int cantClientes=tablaClientes->size();
    int ackRecibidosOk=0;
    if(result >0){ //si se envio el mensaje
      while(cantClientes>ackRecibidosOk){
        rdtMsj* mensajeRcb;
        memset(mensajeRcb , 0, sizeof(&mensajeRcb));
        //envio ACK ok
        memset(&addr, 0, sizeof(addr));
        if ((nbytes = recvfrom(soc, (char*) mensajeRcb, sizeof(*mensajeRcb), 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
          perror("recvfrom");
          exit(1);
        }

        char* ipFrom = inet_ntoa(addr.sin_addr);
        int puerto=ntohs(addr.sin_port);
        char* claveCliente = new char[20];
        sprintf(claveCliente, "%s:%d", ipFrom, puerto);

        //TODO chequear que los nuevos clientes no jodan
        if((mensajeRcb->esAck) && (mensajeRcb->esMulticast) && (mensajeRcb->seq == multicastSeq) && ((*tablaClientes)[claveCliente]==false)){ //TODO chequear que ack correcto
          (*tablaClientes)[claveCliente]=true;
          ackRecibidosOk++;
        }
      }
      if(cantClientes == ackRecibidosOk){
        //Todos los clientes confiramaron la recepcion del mensaje
        multicastSeq = (multicastSeq+1) % 2;
      }
    }
  }
}

//debria recibir como parametro ademas del mensaje
// ip multicast
// puerto multicast
// ip servidor (no seria necesario ya que es la ip que corre)
// puerto del servidor
// lista de clientes logueados
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
  //deberia quedarse en un loop hasta que se reciban todos los mensajes

  // sinRecibir = listaClientes;
  // recibo:
  // setTimeOut(x segundos);
  // recibidos = 0;
  // while(sinRecibir.size>0){
  //   recvfrom(socket_servidor, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, &addrlen)) < 0);
  //   if(paquete.from in sinRecibir && isAckValid(paquete)){
  //     sinRecibir.erase(paquete.from)
  //   }
  // }
  // if(sinRecibir.size>0 && timeout && cantTimeout<MAX_TIMEOUT){
  //   cantTimeout++;
  //   goto recibo;
  // }else{
  //   error, hay al menos (sinrecibir.s)
  // }



}