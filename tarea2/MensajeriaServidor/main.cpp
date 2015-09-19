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

typedef struct Cliente
{
   char  nick[50];   
} Cliente;

typedef map<char*, Cliente*> Clientes;

int prueba(){
    return 0;
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    Clientes* clientes = new Clientes();
    
    for (int i = 0; i < 20; i++) {
        char* ip = new char[20];
        sprintf(ip, "192.168.1.%d", i);
        
        Cliente* c = new Cliente;
        sprintf(c->nick, "Cliente[%d]", i);
        
        clientes->insert(pair<char*, Cliente*>(ip,c));
    }

    cout << "prueba" << endl;
    
    for (Clientes::iterator it=clientes->begin(); it!=clientes->end(); ++it)
        std::cout << it->first << " => " << it->second->nick << '\n';
    return 0;
    
    while(true){
        //lee comandos de una pila
    }
}

