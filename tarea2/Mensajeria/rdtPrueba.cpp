
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

#define DEBUG false

//global vars
string usuario(USUARIO);

string ip_multicast(IP_MULTICAST);
int puerto_multicast(PUERTO_MULTICAST);

string ip_cliente(IP_CLIENTE);
int puerto_cliente(PUERTO_CLIENTE);

string ip_servidor(IP_SERVIDOR);
int puerto_servidor(PUERTO_SERVIDOR);
//fin global vars

// int socket_servidor
// int soc_ser;

void printMensaje(rdtMsj* m){
  std::cout << "From:" << m->from << " esACK:" << m->esAck<< " esMulticast:" << m->esMulticast << " seq:" << m->seq << " mensaje:" << m->mensaje << std::endl;
}

//mapa con clave ip y numero de secuancia
typedef map<string, int> TablaSecuencias;
// typedef struct {
TablaSecuencias* emisor = new TablaSecuencias;
TablaSecuencias* receptor = new TablaSecuencias;


int CrearSocket(int puerto, bool multicast){
  struct sockaddr_in addr;
  struct ip_mreq mreq;

  int sockete = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockete < 0) {
    perror("No se pudo crear el socket");
    exit(0);
  }
  cout << "CrearSocket=> puerto:" << puerto << ", multicast:" << multicast << endl;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(puerto);
  //TODO atender unicamente ip servidor e ip multicast
  addr.sin_addr.s_addr = htonl(INADDR_ANY); //direccion que atiende

  if (bind(sockete, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    cout << "Error al hacer bind " << endl;
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

char* getClienteId(char* ip, int puerto){
  return NULL;
}
char* getIp(char* clienteId){
  return NULL;
}
int getPuerto(char* clienteId){
  return 0;
}

void printTablaCliente(TablaClienteId* tabla){
  TablaClienteId::iterator it = tabla->begin();
  int i = 0;
  while(it != tabla->end()) {
    //lleno la tabla con ["ip:puerto"]=flase
    cout << i <<")TablaClienteId[" << it->first << "]=" << it->second << endl;
    it++;
  }
}

bool getClienteRecibioTablaMulticast(TablaClienteId* tabla, char* clienteId){
  printTablaCliente(tabla);
  cout << "__DEBUG getClienteRecibioTablaMulticast(" << clienteId << ")" << endl;
  TablaClienteId::iterator it = tabla->find(clienteId);
  if(it == tabla->end()){
    return false;
  }
  return true;
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

char* rdt_recibe(int soc, char*& ipEmisor, int& puertoEmisor){
  if(DEBUG) cout << "rdt_recibe inicio " << endl;

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  socklen_t addrlen;
  int nbytes;
  rdtMsj* mensaje = new rdtMsj;

  while(true){
    memset(mensaje , 0, sizeof(*mensaje));
    addrlen  = sizeof(addr);

    //recibimos UDP
    if(DEBUG) cout << "rdt_recibe recibiendo..." << endl;
    if ((nbytes = recvfrom(soc, (char*) mensaje, sizeof(*mensaje), 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
      // if(DEBUG) cout << "__DEBUG ERROR" << endl;
      perror("recvfrom");
      exit(1);
    }
    // cout << "rdt_recibe  nbytes" << nbytes << " sizeof(*mensaje)" << sizeof(*mensaje) << endl;
    char* ipFrom = inet_ntoa(addr.sin_addr);
    int puerto=ntohs(addr.sin_port);
    int seqEsperado = getSequenceNumber(receptor, ipFrom);
    if(DEBUG) std::cout << "rdt_recibe Se recibe desde ipFrom:" << ipFrom << ":" << puerto << std::endl;
    if(DEBUG) cout << "rdt_recibe Se recibe => " << endl;
    if(DEBUG) printMensaje(mensaje);

    rdtMsj* respuesta = new rdtMsj;
    respuesta->esAck=true;
    //respuesta.from = "importa?";
    // respuesta.mensaje="importa munia?";
    respuesta->esMulticast=mensaje->esMulticast;
    //envio ACK ok
    if(mensaje->esMulticast){
      respuesta->seq=mensaje->seq;
    }else{
      respuesta->seq=seqEsperado;
    }

    if(DEBUG)  cout << "rdt_recibe Se envia =>" << endl;
    printMensaje(respuesta);
    addrlen  = sizeof(addr);
    if(DEBUG)  cout << "rdt_recibe Enviando..." << endl ;
    int result = sendto(soc, respuesta, sizeof(*respuesta), 0, (struct sockaddr *)&addr, addrlen);

    if( (result>0) && (seqEsperado == mensaje->seq)){ //(&& result > 0 )
      //actualizo tabla
      int newSeq = (++seqEsperado)%2;
      // it->second = newSeq;
      updateSequenceNumber(receptor, ipFrom, newSeq);
      ipEmisor = new char[MAX_IP_LENGTH];
      strcpy(ipEmisor, ipFrom);
      puertoEmisor = ntohs(addr.sin_port);
      //todo ok :)
      if(DEBUG) printf("rdt_recibe Mensaje recibido: %s\n", mensaje->mensaje);
      char* retMen = new char[MSGBUFSIZE];
      strcpy(retMen, mensaje->mensaje);

      return retMen;
    }

  }

}

void rdt_sendto(int soc, char* mensajeToSend, char* ip, int puerto){

  if (DEBUG) cout << "rdt_sendto=> mensaje: " << mensajeToSend << " ip:" << ip << " puerto:" << puerto;

  int nbytes;
  socklen_t addrlen;
  struct sockaddr_in addr;

  int seqEsperado = getSequenceNumber(emisor, ip);

  while(true){
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(puerto);
    addr.sin_addr.s_addr = inet_addr(ip);

    addrlen  = sizeof(addr);
    // int result = sendto(soc, respuesta, sizeof(respuesta), 0, (struct sockaddr *)&addr, addrlen);

    rdtMsj* mensaje = new rdtMsj;
    memset(mensaje , 0, sizeof(*mensaje));
    mensaje->esAck=false;
    strcpy(mensaje->from, "esNecesario?");
    strcpy(mensaje->mensaje, mensajeToSend);
    mensaje->esMulticast=false;
    mensaje->seq = getSequenceNumber(emisor, ip);

    if (DEBUG) cout << "rdt_send enviando...";
    int result = sendto(soc, (char*) mensaje, sizeof(*mensaje), 0, (struct sockaddr *)&addr, addrlen);

    if(result >0){
      rdtMsj* mensajeRcb = new rdtMsj;
      memset(mensajeRcb , 0, sizeof(*mensajeRcb));
      //envio ACK ok
      if(DEBUG) cout << "rdt_send espero ACK con seq:" << seqEsperado << endl;
      if ((nbytes = recvfrom(soc, (char*) mensajeRcb, sizeof(*mensajeRcb), 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
        perror("recvfrom");
        exit(1);
      }

      //char* ipFrom = inet_ntoa(addr.sin_addr);
      char* ipFrom = inet_ntoa(addr.sin_addr);

      if(DEBUG) cout << "rdt_send recibio mensaje:" ;
      if(DEBUG) printMensaje(mensajeRcb);

      if( (strcmp(ipFrom, ip)==0) && (mensajeRcb->esAck) &&  (mensajeRcb->seq==seqEsperado)){
        //mismo ip
        updateSequenceNumber(emisor, ip, (++seqEsperado)%2);
        printf("DEBUG::Mensaje enviado: %s\n", mensaje->mensaje);
        return ;
      }
      else{
        printf("rdt_send Error:");
        cout << "rdt_send DEBUG values (strcmp(ipFrom, ip)==0): " << (strcmp(ipFrom, ip)==0) <<  " (mensajeRcb->esAck):" << (mensajeRcb->esAck) << " (mensajeRcb->seq==seqEsperado):" <<  (mensajeRcb->seq==seqEsperado) << endl;
      }
    }

  }

}


int multicastSeq=0;
void rdt_send_multicast(int soc, char* mensajeToSend, TablaClienteId* tablaClientes){
  struct sockaddr_in addr;
  int nbytes;
  socklen_t addrlen = sizeof(struct sockaddr_in);

  //armo direccion del multicast
  struct sockaddr_in addr_multicast;
  memset((char *)&addr_multicast, 0, sizeof(addr_multicast));
  addr_multicast.sin_family = AF_INET;
  addr_multicast.sin_port = htons(puerto_multicast);
  addr_multicast.sin_addr.s_addr = inet_addr(ip_multicast.c_str());

  cout << "__DEBUG:rdtSendMulticast:inicializando estructuras" << ip_multicast << ":" << puerto_multicast << endl;
  //armo mensaje a enviar
  rdtMsj* mensaje = new rdtMsj;
  rdtMsj* mensajeRcb = new rdtMsj;
  memset(mensaje , 0, sizeof(*mensaje));
  mensaje->esAck=false;
  strcpy(mensaje->from, "esNecesario?");
  strcpy(mensaje->mensaje, mensajeToSend);
  mensaje->esMulticast=true;
  mensaje->seq = multicastSeq;

  char* prueba = new char[MAX_LARGO_MENSAJE];
  memset(prueba, 0, sizeof(char)*MAX_LARGO_MENSAJE );
  strcpy(prueba, "MESSAGE Debug multicast");


  while(true){
    //envio mensaje a multicast
    cout << "__DEBUG-:RDT sendMulticast, Contendio=>" ;
    printMensaje(mensaje);
    cout << "socket:" << soc << endl;
    int result = sendto(soc, (char*)mensaje, sizeof(*mensaje) , 0, (struct sockaddr *)&addr_multicast, sizeof(addr_multicast));
    //int result = sendto(soc, (char*)prueba , sizeof(char)*MAX_LARGO_MENSAJE , 0, (struct sockaddr *)&addr_multicast, sizeof(addr_multicast));

    int cantClientes=tablaClientes->size();
    int ackRecibidosOk=0;
    if(result >0){ //si se envio el mensaje

      while(cantClientes>ackRecibidosOk){
        memset(mensajeRcb , 0, sizeof(*mensajeRcb));
        memset(&addr, 0, sizeof(struct sockaddr_in));
        cout << "__DEBUG espero ack" << endl;
        if ((nbytes = recvfrom(soc, (char*) mensajeRcb, sizeof(*mensajeRcb), 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
          cout << "Error extranio" << endl;
          perror("recvfrom");
          exit(1);
        }
        cout << "__DEBUG-:RDT send muticas, RECIBI=>" ;
        printMensaje(mensajeRcb);

        char* ipFrom = inet_ntoa(addr.sin_addr);
        int puerto=ntohs(addr.sin_port);
        char* claveCliente = new char[20];
        sprintf(claveCliente, "%s:%d", ipFrom, puerto);

        bool clienteEsta = getClienteRecibioTablaMulticast(tablaClientes, claveCliente);
        std::cout << "__DEBUG ipFrom:" << ipFrom << " puerto:" << puerto << " claveCliente:" << claveCliente << std::endl;
        std::cout << "(mensajeRcb->esAck):" << (mensajeRcb->esAck) <<  " (mensajeRcb->esMulticast):" << (mensajeRcb->esMulticast) <<  " (mensajeRcb->seq == multicastSeq):" << (mensajeRcb->seq == multicastSeq) << " (clienteEsta):" << (clienteEsta)  << std::endl;
        if((mensajeRcb->esAck) && (mensajeRcb->esMulticast) &&
        (mensajeRcb->seq == multicastSeq) && (clienteEsta) && !((*tablaClientes)[claveCliente]) ){
          std::cout << "__ DEBUG send_multicast encontre un cliente" << std::endl;
          (*tablaClientes)[claveCliente]=true;
          ackRecibidosOk++;
        }
      }
      if(cantClientes == ackRecibidosOk){
        //Todos los clientes confiramaron la recepcion del mensaje
        multicastSeq = (multicastSeq+1) % 2;
        printf("Mensaje multicast enviado\n");
        return;
      }
    }
    else{//FIXME
      cout << "send fail" << endl;
      sleep(3);
    }
  }
  delete(mensaje);
  delete(mensajeRcb);
}

// //debria recibir como parametro ademas del mensaje
// // ip multicast
// // puerto multicast
// // ip servidor (no seria necesario ya que es la ip que corre)
// // puerto del servidor
// // lista de clientes logueados
// void sendMulticast(char* mensaje){
//   char buffer[MSGBUFSIZE];
//   bzero(buffer,MSGBUFSIZE);
//   strcat(buffer, mensaje);
//
//   struct sockaddr_in addr_multicast;
//   memset((char *)&addr_multicast, 0, sizeof(addr_multicast));
//   addr_multicast.sin_family = AF_INET;
//   addr_multicast.sin_port = htons(puerto_multicast);
//   addr_multicast.sin_addr.s_addr = inet_addr(ip_multicast.c_str());
//
//   int result = sendto(socket_servidor, buffer, strlen(buffer), 0, (struct sockaddr *)&addr_multicast, sizeof(addr_multicast));
//
//   if (result < 0) {
//     cout << "algo no salio bien cuando mandamos el mensaje." << endl;
//     perror("algo no salio bien cuando mandamos el mensaje.");
//     exit(1);
//   } else {
//     printf("se enviaron %d bytes\n", result);
//   }
//   //deberia quedarse en un loop hasta que se reciban todos los mensajes
//
//   // sinRecibir = listaClientes;
//   // recibo:
//   // setTimeOut(x segundos);
//   // recibidos = 0;
//   // while(sinRecibir.size>0){
//   //   recvfrom(socket_servidor, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, &addrlen)) < 0);
//   //   if(paquete.from in sinRecibir && isAckValid(paquete)){
//   //     sinRecibir.erase(paquete.from)
//   //   }
//   // }
//   // if(sinRecibir.size>0 && timeout && cantTimeout<MAX_TIMEOUT){
//   //   cantTimeout++;
//   //   goto recibo;
//   // }else{
//   //   error, hay al menos (sinrecibir.s)
//   // }
//
// }
