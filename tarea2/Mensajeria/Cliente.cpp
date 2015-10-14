/*
 * File:   main.cpp
 * Author: emi
 *
 * Created on 19 de septiembre de 2015, 12:33 PM
 */

// C
#include <string.h>
#include <stdio.h>
#include <stdlib.h> //atoi
#include <unistd.h> //sleep
#include <pthread.h>

// C++
#include <string>
#include <iostream>

// definido por nosotos
#include "rdtPrueba.h"
//#include "rdt.h"

using namespace std;

// mensajes del servidor al cliente
#define RELAYED_MESSAGE "RELAYED_MESSAGE"
#define PRIVATE_MESSAGE "PRIVATE_MESSAGE"
#define CONNECTED "CONNECTED"
#define GOODBYE "GOODBYE"

// mensajes del cliente al servidor
#define LOGIN "LOGIN"
#define LOGOUT "LOGOUT"
#define GET_CONNECTED "GET_CONNECTED"
#define MESSAGE "MESSAGE"
#define PRIVATE_MESSAGE "PRIVATE_MESSAGE"

// puertos para los sockets del cliente
#define PUERTO_COMANDO 9999


// constantes
#define MAX_LARGO_MENSAJE 200
#define MAX_TEXTO 160
#define MAX_NICK 20

//variables globales
char* apodo;
char* IPservidor;
int puertoServidor;
int socketMensajes;
int socketComandos;

bool meVoy;

void * receptorMensajes(void*) {
  printf("Empiezo a recibir mensajes\n");
  char* comando;
  char* ipEmisor;
  int puertoEmisor;
  int i = 0;
  int contCon = 0;
  while (!meVoy) {
    char* result;
    
    //recibo y parseo mensaje, se bloquea hasta que recibe un mensaje
    cout << "--Espero por mensaje..." << endl;
    comando = rdt_recibe(socketMensajes, ipEmisor, puertoEmisor);
    printf("--Mensaje (%d) recibido :: %s ::\n", i, comando);
    printf("--Origen del mensaje: %s:%d\n", ipEmisor, puertoEmisor);
    string cmd = comando;
    if (cmd.find(RELAYED_MESSAGE) == 0) {
      cout << "Mensaje multicast: " << endl;
      result = strtok(comando," ");
      result = strtok(NULL," ");
      cout << "\tEmisor: " << result << endl;
      result = strtok(NULL,"\n");
      cout << "\tMensaje: " << result << endl;
    } else if (cmd.find(PRIVATE_MESSAGE) == 0) {
      cout << "Mensaje privado: " << endl;
      result = strtok(comando," ");
      result = strtok(NULL," ");
      cout << "\tEmisor: " << result << endl;
      result = strtok(NULL,"\n");
      cout << "\tMensaje: " << result << endl;
    } else if (cmd.find(CONNECTED) == 0) {
      result = strtok(comando, " ");
      printf("Usuarios conectados:\n");
      result = strtok(NULL, "|");
      contCon=0;
      while (result != NULL) {
        printf("\t%d)%s\n", contCon++, result);
        result = strtok(NULL, "|");
      }
    } else if (cmd.find( GOODBYE) == 0) {
      printf("Me desconectaron: %s\n",comando);
      //FIXME
      meVoy = true;
    } else{
      printf("Comando <%s> no reconocido.", comando);
    }
    i++;
  }
  return NULL;
}

void mensajeria() {

  // Hilo 1: para recivir los mensajes de los clientes, rdt_rcv();
  pthread_t idHilo;
  pthread_create(&idHilo, NULL, receptorMensajes, NULL);

  sleep(1);
  printf("++Empiezo a enviar comandos\n");

  // Hilo 2: para enviar comandos al servidor rdt_send();
  //inicializo
  char* mensaje = new char[MAX_LARGO_MENSAJE];
  char* texto = new char[MAX_TEXTO];
  char* nick = new char[MAX_NICK];
  bool error;

  string comando;
  while (!meVoy) {
    //leo de pantalla estandar un comando
    cout << "++Ingrese comando >";
    getline(cin, comando);
    error = false;
    //parseo comando

    if (comando.find(LOGOUT) == 0) {
      meVoy = true;
      sprintf(mensaje, "%s%s", LOGOUT, CR);
      //tengo que liberar recursos?
    } else if (comando.find(GET_CONNECTED) == 0) {
      //pido lista de clientes conectados
      sprintf(mensaje, "%s%s", GET_CONNECTED, CR);
    } else if (comando.find(MESSAGE) == 0) {
      //envia un mensaje a todos los clientes
      sscanf(comando.data(), "%*s %[^\n]", texto);
      sprintf(mensaje, "%s %s%s", MESSAGE, texto, CR);
      //hay que setear el mensaje
    } else if (comando.find(PRIVATE_MESSAGE) == 0) {
      //envia un mensaje privado
      sscanf(comando.data(), "%*s %s %[^\n]", nick, texto);
      sprintf(mensaje, "%s %s %s%s", PRIVATE_MESSAGE, nick, texto, CR);
      //hay que setear el mensaje
    } else {
      sprintf(mensaje, "Error!");
      error = true;
    }
    printf("++Se ingreso el comando %s\n", mensaje);
    // si no hubo error envio el mensaje al servidor
    if (!error) {
      cout << "++Enviando:::" << mensaje << ":::" << endl;
      rdt_sendto(socketComandos, mensaje, IPservidor, puertoServidor);
      cout << "++Mensaje Enviado " << endl;
    } else {
      cout << "comando no reconocido" << endl;
    }
  }
}

/*
 *
 */
int main(int argc, char** argv) {

  // cargo los parametros (apodo, IP y puerto del servidor)
  int puertoComandos(PUERTO_COMANDO);
  int puertoMensajes(PUERTO_MENSAJES_CLI);

  if (argc < 4) {
    printf("Uso: ./cliente <apodo> <IP servidor> <puerto servidor>\n");
    exit(0);
  }
  apodo = argv[1];
  IPservidor = argv[2];
  puertoServidor = atoi(argv[3]);

  // creo los sockets que va a usar
  cout << endl <<"socketMensajes-receptor" ;
  socketMensajes = CrearSocket(puertoMensajes, true);
  cout << "socketComandos-emisor" ;
  socketComandos = CrearSocket(puertoComandos, false);
  cout << endl;

  if (socketMensajes < 0 || socketComandos < 0) {
    printf("No se pudo iniciar el cliente");
    exit(0);
  }

  // Me logueo, armo el mensaje para el servidor con mis datos (IP y puerto).
  char login[MAX_LARGO_MENSAJE];
  memset(&login, 0, MAX_LARGO_MENSAJE);
  strcat(login, "LOGIN ");
  strcat(login, apodo);
  strcat(login, CR);


  printf("Mensaje de logueo: %s\n", login);
  rdt_sendto(socketComandos, login, IPservidor, puertoServidor);
  printf("Su nombre de usuario es: %s y su servidor es %s:%d\n", apodo, IPservidor,puertoServidor);
  meVoy = false;
  // Empiezo a enviar y recibir mensajes (hasta LOGOUT)
  mensajeria();
  //rdt_cerrarSocket(socketComandos);
  //rdt_cerrarSocket(socketMensajes);

  return 0;
}
