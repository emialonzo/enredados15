/*
 * File:   main.cpp
 * Author: emi
 *
 * Created on 19 de septiembre de 2015, 10:27 AM
 */

#include <stdio.h>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <string.h>
#include <time.h>
time_t start = time(0);
using namespace std;

#include "rdt.h"
#include "constantes.h"

//FIXME esto es para probar tiene que volar
#include "rdt_test.h"

#define RELAYED_MESSAGE "RELAYED_MESSAGE"
#define PRIVATE_MESSAGE "PRIVATE_MESSAGE"
#define CONNECTED "CONNECTED"
#define GOODBYE "GOODBYE"


typedef struct Cliente {
        char nick[50];
        int puerto;
        char ip[20];
        int cantMensajes;//Es neecesario?
        time_t ult_actividad;
} Cliente;
typedef struct Servidor {
        int cantMensajes;
        int cantClientes;
} Servidor;
typedef struct Mensaje {
        char  destino[MAX_IP_LENGTH];
        int   dest_puerto;
        char  origen[MAX_IP_LENGTH];
        int   orig_puerto;
        char  msg[MAX_TEXTO];
        bool  multicast;

} Mensaje;
typedef map<string, Cliente*> MapClientes;

typedef enum  {COM_LOGIN, COM_LOGOUT, COM_GET_CONNECTED, COM_MSG, COM_PVT_MSG, COM_INVALID} MsgComand;

MapClientes* Clientes = new MapClientes;
Servidor* servidor = new Servidor;

typedef queue<Mensaje*> ColaMensajes;

ColaMensajes* colaMensajes = new ColaMensajes;

char* ipServidor = IP_SERVIDOR;
int puertoServidor;

char* ipMulticast = IP_MULTICAST;
int puertoMulticast = PUERTO_MULTICAST;

pthread_mutex_t queueMutex;
pthread_cond_t emitCond;

pthread_mutex_t clientesMutex;

int socEmisor = 0;
int socReceptor = 0;

void loginCliente(Cliente* c) {
        Clientes->insert(make_pair(c->ip, c));
}
void logOut(Cliente* c){
        Clientes->erase(c->ip);
};

Cliente* getCliente(char* ip){
        try {
                return Clientes->at(ip);
        }catch(...) {
                cout <<
                "cliente no existe" << endl;
                return NULL;
        }
}


//Se tiene que llamar con el mutex de clientes ya pedido
Cliente* getClienteByNick(const char* nick) {
        Cliente * ret = NULL;
        map<string, Cliente*>::iterator iter = Clientes->begin();

        while (iter != Clientes->end()) {
                ret = iter->second;

                if (strcmp(ret->nick, nick) == 0) {
                        return ret;
                }
                ++iter;
        }

        return ret;
}



Mensaje* crearMensaje(char* ipDestino,int puerto, bool multicast, char* contenido) {

        Mensaje* ret = new Mensaje();
        strcpy(ret->origen,IP_SERVIDOR);
        strcpy(ret->msg, contenido);
        strcpy(ret->destino, ipDestino);

        ret->dest_puerto = puerto;
        ret->multicast = multicast;

        return ret;
}

void encolarMensaje(Mensaje* mensaje) {
        pthread_mutex_lock(&queueMutex);
        colaMensajes->push(mensaje);
        pthread_cond_signal(&emitCond);
        pthread_mutex_unlock(&queueMutex);
}

char* getConected(){
        char* retStr = new char[MAX_LARGO_MENSAJE];
        MapClientes::iterator it = Clientes->begin();
        //TODO si el diccionario esta vacio revienta
        if(Clientes->size()==1){
          strcat(retStr, it->second->nick);
        }
        else{
          strcat(retStr, it->second->nick);
          ++it;
          while(it != Clientes->end()) {
                  strcat(retStr, "|");
                  strcat(retStr, it->second->nick);
                  ++it;
          }
        }

        return retStr;
}

int processGetConnectedMsg(char* ip, int puerto) {
        char contenido[MAX_TEXTO];
        pthread_mutex_lock(&clientesMutex);
        sprintf(contenido, "%s %s", CONNECTED, getConected());
        pthread_mutex_unlock(&clientesMutex);
        Mensaje* mensaje = crearMensaje(ip, puerto, false, contenido);
        encolarMensaje(mensaje);
        return 0;
}

int processLoginMsg(char* ip, int puerto, char * msg) {

        pthread_mutex_lock(&clientesMutex);
        if (Clientes->find(ip) == Clientes->end()) {
                Cliente * cli = new Cliente();
                string nick = msg;
                nick = nick.substr(nick.find(" ") + 1);
                strcpy(cli->nick, nick.c_str());
                strcpy(cli->ip, ip);
                cli->puerto = puerto;
                cli->ult_actividad = time(0);

                Clientes->insert(make_pair(cli->ip, cli));
                pthread_mutex_unlock(&clientesMutex);
                return 0;
        }
        pthread_mutex_unlock(&clientesMutex);
        return -1;
}

int processLogut(char* ip, int puerto) {
        //FIXME aca hay que
        pthread_mutex_lock(&clientesMutex);
        Clientes->erase(ip);
        pthread_mutex_unlock(&clientesMutex);
        char contenido[MAX_TEXTO] = GOODBYE;
        Mensaje* mensaje = crearMensaje(ip, puerto, false, contenido);
        encolarMensaje(mensaje);
        return 0;
}

int processMulticastMessage(char* sourceIp, char* recv_msg) {
        pthread_mutex_lock(&clientesMutex);
        map<string, Cliente*>::iterator iter = Clientes->find(sourceIp);
        if (iter != Clientes->end()) {
                Cliente* cli = iter->second;
                pthread_mutex_unlock(&clientesMutex);
                string str_contenido = recv_msg;
                str_contenido = str_contenido.substr(str_contenido.find(" ") +1);

                char contenido[MAX_TEXTO];
                sprintf(contenido, "%s %s %s", RELAYED_MESSAGE, cli->nick, str_contenido.c_str());

                Mensaje* mensaje = crearMensaje(ipMulticast, puertoMulticast, true, contenido);
                encolarMensaje(mensaje);
                return 0;

        }
        pthread_mutex_unlock(&clientesMutex);
        return -1;
}

int processPrivatetMessage(char* sourceIp, char* recv_msg) {

        pthread_mutex_lock(&clientesMutex);
        map<string, Cliente*>::iterator iter = Clientes->find(sourceIp);

        if (iter != Clientes->end()) {

                Cliente* cli = iter->second;
                string str_recv_msg = recv_msg;

                //Descarto el cabezal private msg
                str_recv_msg = str_recv_msg.substr(str_recv_msg.find(" ") +1);

                //Nick esta desde el inicio hasta el primer espacio
                const char* dest_nick = str_recv_msg.substr(0, str_recv_msg.find(" ")).c_str();

                Cliente* dest_cli = getClienteByNick(dest_nick);
                if (dest_cli != NULL) {
                        //Descarto el nick y me quedo con el mensaje
                        str_recv_msg = str_recv_msg.substr(str_recv_msg.find(" ") +1);

                        char contenido[MAX_TEXTO];
                        sprintf(contenido, "%s %s %s", PRIVATE_MESSAGE, cli->nick, str_recv_msg.c_str());

                        Mensaje* mensaje = crearMensaje(dest_cli->ip, dest_cli->puerto, false, contenido);
                        encolarMensaje(mensaje);
                        pthread_mutex_unlock(&clientesMutex);
                        return 0;
                }
        }
        pthread_mutex_unlock(&clientesMutex);
        return -1;
}



void parseMessage(Cliente* c, char* mensaje){
        string comando = mensaje;

        if (comando.find(LOGIN) == 0) {
                //obtengo nombre de usuario
                strcpy(c->nick,mensaje);
                loginCliente(c);
        } else if (comando.find(LOGOUT) == 0) {
                //desloegueo al usuario
                logOut(c);
        } else if (comando.find(GET_CONNECTED) == 0) {
                //envio conectados
                //char* conectados = getConected();
        } else if (comando.find(MESSAGE) == 0) {
                //envio mensaje multicast
        } else if (comando.find(PRIVATE_MESSAGE) == 0) {
                //envio mensaje privado
        } else {
                std::cout << "Ha llegado un mensaje invallido hacia el servidor." << std::endl;
        }
}

MsgComand getCommandFromMsg(char* msg) {
        string comando = msg;
        MsgComand ret = COM_INVALID;

        if (comando.find(LOGIN) == 0) {
                ret = COM_LOGIN;
        } else if (comando.find(LOGOUT) == 0) {
                ret = COM_LOGOUT;
        } else if (comando.find(GET_CONNECTED) == 0) {
                ret = COM_GET_CONNECTED;
        } else if (comando.find(MESSAGE) == 0) {
                ret = COM_MSG;
        } else if (comando.find(PRIVATE_MESSAGE) == 0) {
                ret = COM_PVT_MSG;
        }

        return ret;
}



void* debug(){
        cout << "::DEBUG:: escribe ip de cliente" << endl;
        string ip;
        getline(cin, ip);
        cout << "::DEBUG:: escribe un mensaje que llega desde cliente" << endl;
        cout << ">";
        string comando;
        getline(cin, comando);

        char * cstrComando = new char [comando.length()+1];
        strcpy (cstrComando, comando.c_str());

        char * cstrIp = new char [comando.length()+1];
        strcpy (cstrIp, comando.c_str());

        Cliente* c = getCliente(cstrIp);


        parseMessage(c, cstrComando);
        return NULL;
};

void* debugRdt(){
        cout << ">";
        string comando;
        getline(cin, comando);
        char* mensaje = new char[MAX_LARGO_MENSAJE];
        strcpy(mensaje, comando.c_str());
        sendMulticast(mensaje);
        return NULL;
};

void clientesConectados(){
        //TODO
        cout << "TODO" << endl;
}
void mensajesEnviados(){
        //TODO
        cout << "TODO" << endl;
}
void conexionesTotales(){
        //TODO
        cout << "TODO" << endl;
}
void tiempoEjecucion(){
        double seconds_since_start;
        seconds_since_start = difftime( time(0), start);
        std::cout << "Han pasado "  << seconds_since_start << " segundos" << std::endl;
}

int consola() {
        char c;

        do {
          cout << ">" ;
                string comando;
                getline(cin, comando);
                c=comando.c_str()[0];
                if (c=='a') {
                        // * a – cantidad de clientes conectados -- clientes logueados
                        clientesConectados();
                        /* code */
                } else if (c=='s') {
                        // * s – cantidad de mensajes enviados -- lista de mensajes
                        mensajesEnviados();
                        /* code */
                } else if (c=='d') {
                        // * d – cantidad de conexiones totales
                        conexionesTotales();
                        /* code */
                } else if (c=='f') {
                        // * f – tiempo (wall time) de ejecución
                        tiempoEjecucion();
                } else if (c=='x') {
                        // * f – tiempo (wall time) de ejecución
                        cout << "DEBUG::" ;
                        debugRdt();
                } else {
                        cout << "Comando no reconocido." << endl;
                }
        } while (c != 'e');
        return 0;
}

void* emisorMensajes(void*) {
        //FIXME aca no tengo claro que pasarle.
        socEmisor = crearSocket(puertoMulticast, false);
        while (true) {
                //mutex_lock
                pthread_mutex_lock(&queueMutex);
                if (colaMensajes->empty()) {
                        //wait_cond
                        pthread_cond_wait(&emitCond,&queueMutex);
                        //cout << "Emisor liberado" << endl;
                }

                Mensaje * msg = colaMensajes->front();

                colaMensajes->pop();
                //mutex_unlock
                pthread_mutex_unlock(&queueMutex);

                appMsg* rdt_msg = new appMsg();
                strcpy(rdt_msg->mensaje, msg->msg);
                strcpy(rdt_msg->source_ip, msg->origen);

                if (msg->multicast) {
                        //FIXME esto es de test aca se hce multicast
                        test_rdt_send_broadcast(socEmisor, rdt_msg, msg->destino, msg->dest_puerto);
                }
                else {
                        //FIXME esto es de test aca se hace unicast
                        test_rdt_send(socEmisor, rdt_msg, msg->destino, msg->dest_puerto);
                }

        }
        return NULL;
}


void* receptorMensajes(void*) {
        socReceptor = crearSocket(puertoServidor, false);
        while (true) {
                //FIXME esto es un test, aca va rdt_rvc
                appMsg* msg = test_rdt_rcv(socReceptor);

                MsgComand command = getCommandFromMsg(msg->mensaje);

                switch (command) {
                        case COM_LOGIN:
                                processLoginMsg(msg->source_ip, msg->source_port, msg->mensaje);
                                break;
                        case COM_GET_CONNECTED:
                                processGetConnectedMsg(msg->source_ip, msg->source_port);
                                break;
                        case COM_MSG:
                                processMulticastMessage(msg->source_ip, msg->mensaje);
                                break;
                        case COM_PVT_MSG:
                                processPrivatetMessage(msg->source_ip, msg->mensaje);
                                break;
                        case COM_LOGOUT:
                                processLogut(msg->source_ip, msg->source_port);
                                break;
                        case COM_INVALID:
                                //TODO ver que se hace con un caracter valido
                                break;
                }
        }
        return NULL;
}


/*
 *
 */

void init() {
        iniServer();
        test_init();

        pthread_mutex_init(&queueMutex, NULL);
        pthread_cond_init (&emitCond, NULL);
        pthread_mutex_init(&clientesMutex, NULL);


}

int main(int argc, char** argv) {

        init();

        pthread_t receptorHilo;
        pthread_create(&receptorHilo, NULL, receptorMensajes, NULL);

        pthread_t emisorHilo;
        pthread_create(&emisorHilo, NULL, emisorMensajes, NULL);

        consola();

        return 0;
}
