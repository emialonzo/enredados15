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
using namespace std;


typedef struct Cliente {
    char nick[50];
    char ip[20];
    int cantMensajes;

} Cliente;
typedef struct Servidor {
    int cantMensajes;
} Servidor;
typedef map<char*, Cliente*> MapClientes;

map<char*, Cliente*> Clientes = new map();

int prueba() {
    return 0;
}

void loginCliente(Cliente* c) {
    Clientes.insert(make_pair(c->ip, c));
}
void logOut(Cliente* c){
    Clientes.erase(c->ip);
};

Cliente* getCliente(char* ip){
    try {
        return Clientes.at(ip);
    }catch(...){
        cout << "cliente no existe" << endl;
        return NULL;
    }
}

void enviarMensajePrivado(Cliente from, Cliente to){
    throw "TO-DO";
};

void enviarMensajePrivado(char* ipFrom, char* ipTo){
    Cliente* cFrom = getCliente(ipFrom);    
    Cliente* cTo = getCliente(ipTo);    
    enviarMensajePrivado(cFrom, cTo);
}


/*
 * 
 */
int main(int argc, char** argv) {

    //Clientes* clientes = new Clientes();
    MapClientes clientes = Clientes;

    for (int i = 0; i < 20; i++) {
        char* ip = new char[20];
        sprintf(ip, "192.168.1.%d", i);

        Cliente* c = new Cliente;
        sprintf(c->nick, "Cliente[%d]", i);

        clientes->insert(pair<char*, Cliente*>(ip, c));
    }

    cout << "prueba" << endl;

    for (MapClientes::iterator it = clientes->begin(); it != clientes->end(); ++it)
        std::cout << it->first << " => " << it->second->nick << '\n';
    return 0;

    while (true) {
        //lee comandos de una pila
    }
}

