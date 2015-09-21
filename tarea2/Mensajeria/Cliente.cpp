/*
 * File:   main.cpp
 * Author: emi
 *
 * Created on 19 de septiembre de 2015, 12:33 PM
 */

//c
#include <string.h>
#include <stdio.h>
#include <unistd.h> //sleep

#include <pthread.h>




//c++
#include <string>
#include <iostream>

using namespace std;

#define RELAYED_MESSAGE "RELAYED_MESSAGE"
#define PRIVATE_MESSAGE "PRIVATE_MESSAGE"
#define CONNECTED "CONNECTED"
#define GOODBYE "GOODBYE"

#define PRIVATE_MESSAGE "PRIVATE_MESSAGE"
#define GET_CONNECTED "GET_CONNECTED"
#define MESSAGE "MESSAGE"

#define CLIENTE_PRIVATE_MESSAGE "privado"
#define CLIENTE_GET_CONNECTED "conectados"
#define CLIENTE_MESSAGE "mensaje"


#define MAX_LARGO_MENSAJE 200
#define MAX_TEXTO 160
#define MAX_NICK 20
//#define CR "\n"
#define CR "<CR>"

const char* TestCommands[] = {CONNECTED, RELAYED_MESSAGE, PRIVATE_MESSAGE, RELAYED_MESSAGE, "Wednesday", RELAYED_MESSAGE, PRIVATE_MESSAGE, GOODBYE};
const int NumberTest = 8;

//variables globales

void * receptorMensajes(void*) {
    int i = 0;
    //char* comando; //= new char[50];
    const char* comando; //= new char[50];

    while (i < NumberTest) {
        //while (true) {

        //recibo mensaje, se bloquea hasta que recibe un mensaje
        //parseo mensaje
        comando = TestCommands[i];
        if (strcmp(comando, RELAYED_MESSAGE) == 0) {
            //obtenego emisor
            //obtengo mensaje
            //modifico estructoruas
        } else if (strcmp(comando, PRIVATE_MESSAGE) == 0) {
            //obtenego emisor
            //obtengo mensaje
            //modifico estructoruas
        } else if (strcmp(comando, CONNECTED) == 0) {
            //obtiene los clientes conectados separados por pipe
            //parsea lista de clientes conectados
            //altera estructura

        } else if (strcmp(comando, GOODBYE) == 0) {
            //servidor se fue
            //cierro cliente de forma segura

        }

        cerr << ":::DEBUGS_HILOS:::" << comando << endl;
        unsigned int seconds = 4;
        sleep(seconds);
        i++;

    }
    
    return NULL;
}


//prototipo

void sendToServer(char* mensaje) {
    cout << "Enviando:::" << mensaje << ":::" << endl;
    unsigned int seconds = 2;
    sleep(seconds);
    cout << "Mensaje enviado" << endl;
}

int consola() {
    //inicializo
    char* mensaje = new char[MAX_LARGO_MENSAJE];
    char* texto = new char[MAX_TEXTO];
    char* nick = new char[MAX_NICK];
    bool error;

    string comando;

    pthread_t idHilo;
    pthread_create(&idHilo, NULL, receptorMensajes, NULL);

    while (true) {
        //leo de pantalla estandar un comando
        cout << ">";
        getline(cin, comando);
        error = false;
        //parseo comando
        if (comando.find(CLIENTE_GET_CONNECTED) == 0) {
            //pido lista de clientes conectados
            sprintf(mensaje, "%s%s", GET_CONNECTED, CR);
        } else if (comando.find(CLIENTE_MESSAGE) == 0) {
            //envia un mensaje a todos los clientes
            sscanf(comando.data(), "%*s %[^\n]", texto);
            sprintf(mensaje, "%s %s%s", MESSAGE, texto, CR);
            //hay que setear el mensaje
        } else if (comando.find(CLIENTE_PRIVATE_MESSAGE) == 0) {
            //envia un mensaje privado
            sscanf(comando.data(), "%*s %s %[^\n]", nick, texto);
            sprintf(mensaje, "%s <%s> %s%s", PRIVATE_MESSAGE, nick, texto, CR);
            //hay que

        } else {
            // error
            error = true;
        }
        //envia
        if (!error)
            //cout << "Enviando:::" << mensaje << ":::" << endl;
            sendToServer(mensaje);
        else
            cout << "comando no reconocido" << endl;
    }
}

/*
 *
 */
int main(int argc, char** argv) {

    consola();
    return 0;
}
