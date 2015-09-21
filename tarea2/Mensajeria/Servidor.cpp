/*
 * File:   main.cpp
 * Author: emi
 *
 * Created on 19 de septiembre de 2015, 10:27 AM
 */

#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <string.h>
#include <time.h>
time_t start = time(0);
using namespace std;

#include "rdt.h"
#include "constantes.h"


typedef struct Cliente {
        char nick[50];
        char ip[20];
        int cantMensajes;
} Cliente;
typedef struct Servidor {
        int cantMensajes;
        int cantClientes;
} Servidor;
typedef map<char*, Cliente*> MapClientes;

MapClientes* Clientes = new MapClientes;
Servidor* servidor = new Servidor;

int prueba() {
        return 0;
}

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

char* getConected(){
        char* retStr = new char[MAX_LARGO_MENSAJE];
        // for(int i=0; i < servidor->cantClientes;i++){
        //
        // }
        MapClientes::iterator it = Clientes->begin();
        while(it != Clientes->end()) {
                //FIXME arreglar el ultimo pipe
                strcat(retStr, it->second->nick);
                strcat(retStr, "|");
                ++it;
        }

        return retStr;
}

void enviarMensajePrivado(Cliente* from, Cliente* to){
        throw "TO-DO";
};

void enviarMensajePrivado(char* ipFrom, char* ipTo){
        Cliente* cFrom = getCliente(ipFrom);

        Cliente* cTo = getCliente(ipTo);
        enviarMensajePrivado(cFrom, cTo);
}

// LOGIN usuario _CR_
// LOGOUT CR
// GET_CONNECTED CR
// MESSAGE _msg _CR_
// PRIVATE_MESSAGE receptor _msg_ _CR_

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
                char* conectados = getConected();
        } else if (comando.find(MESSAGE) == 0) {
                //envio mensaje multicast
        } else if (comando.find(PRIVATE_MESSAGE) == 0) {
                //envio mensaje privado
        } else {
                std::cout << "Ha llegado un mensaje invallido hacia el servidor." << std::endl;
        }
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
};

void* debugRdt(){
        cout << ">";
        string comando;
        getline(cin, comando);
        char* mensaje = new char[MAX_LARGO_MENSAJE];
        strcpy(mensaje, comando.c_str());
        sendMulticast(mensaje);

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
        //TODO
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
                        cout << "DEBUG::" << endl;
                        debugRdt();
                } else {
                        cout << "Comando no reconocido." << endl;
                }
        } while (c != 'e');
        return 0;
}





/*
 *
 */
int main(int argc, char** argv) {

        //Clientes* clientes = new Clientes();
        MapClientes* clientes = Clientes;
        iniServer();
        consola();

        for (int i = 0; i < 20; i++) {
                char* ip = new char[20];
                sprintf(ip, "192.168.1.%d", i);

                Cliente* c = new Cliente;
                sprintf(c->nick, "nick_%d", i);

                clientes->insert(pair<char*, Cliente*>(ip, c));
        }

        cout << "prueba" << endl;
        char* bufferPrueba = new char[50];

        for (MapClientes::iterator it = clientes->begin(); it != clientes->end(); ++it) {
                sprintf(bufferPrueba, "prueba <%s>", it->first);
                rdt_send(bufferPrueba);
                std::cout << it->first << " => " << it->second->nick << '\n';
        }

        cout << getConected() << endl;

        return 0;

        while (true) {
                //lee comandos de una pila
        }
}
