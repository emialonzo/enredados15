#include "rdt_test.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h> //sleep


#define TOPE 8

appMsg* mensajes[TOPE];



struct expectedMensaje {
        char msg[MAX_LARGO_MENSAJE];
        char dest[12];
        int puerto;
};

expectedMensaje* expected[TOPE];

bool assertMensaje(char * msg, char* dest, int puerto, expectedMensaje* expMsg) {
        bool ret = true;

        if(strcmp(msg, expMsg->msg)) {
                std::cout << "Mensaje esperado:" << std::endl;
                std::cout << expMsg->msg << std::endl;
                std::cout << "Mensaje recibido:" << std::endl;
                std::cout <<msg << std::endl;
                ret = false;
        }

        if(strcmp(dest, expMsg->dest)) {
                std::cout << "Destino esperado:" << std::endl;
                std::cout << expMsg->dest << std::endl;
                std::cout << "Destino recibido:" << std::endl;
                std::cout << dest << std::endl;
                ret = false;
        }

        if (puerto != expMsg->puerto) {
                std::cout << "Puerto esperado:" << std::endl;
                std::cout << expMsg->puerto << std::endl;
                std::cout << "Puerto recibido:" << std::endl;
                std::cout << puerto << std::endl;
                ret = false;
        }

        if (!ret) {
                std::cout << "ERROR AL RECBIR MENSAJE" << std::endl;
        }

        return ret;

}

int paso;
int test;

int crearSocket(int puerto, bool multicast) {
        return 0;
}

appMsg* armarMensaje(const char* ip, int puerto, const char* mensaje) {
        appMsg* ret = new appMsg();
        ret->source_port = puerto;
        strcpy(ret->mensaje, mensaje);
        strcpy(ret->source_ip, ip);
        return ret;
}

expectedMensaje* armarExpected(const char * msg, const char* dest, int puerto) {
        expectedMensaje* ret = new expectedMensaje();
        strcpy(ret->dest, dest);
        ret->puerto = puerto;
        strcpy(ret->msg, msg);
        return ret;
}




void initMenasjes() {
        mensajes[0] = armarMensaje("127.0.0.1", 1234, "LOGIN Tribilin");
        mensajes[1] = armarMensaje("127.0.0.1", 1234, "GET_CONNECTED");
        mensajes[2] = armarMensaje("127.0.0.1", 1234, "MESSAGE Hola como va?");
        mensajes[3] = armarMensaje("222.36.1.45", 6666, "LOGIN Pep");
        mensajes[4] = armarMensaje("222.36.1.45", 6666, "GET_CONNECTED");
        mensajes[5] = armarMensaje("222.36.1.45", 6666, "PRIVATE_MESSAGE Tribilin Esto es peñarol");
        mensajes[6] = armarMensaje("222.36.1.45", 6666, "LOGOUT");
        mensajes[7] = armarMensaje("127.0.0.1", 1234, "GET_CONNECTED");
}

void initExpexted() {
        expected[0] = armarExpected("CONNECTED Tribilin", "127.0.0.1", 1234);
        expected[1] = armarExpected("RELAYED_MESSAGE Tribilin Hola como va?", "225.5.4.48", 8888);
        expected[2] = armarExpected("CONNECTED Tribilin|Pep", "222.36.1.45", 6666);
        expected[3] = armarExpected("PRIVATE_MESSAGE Pep Esto es peñarol", "127.0.0.1", 1234);
        //expected[4] = armarExpected("GOODBYE", "222.36.1.45", 6666);
        expected[4] = armarExpected("CONNECTED Tribilin", "127.0.0.1", 1234);
        expected[5] = armarExpected("GOODBYE", "127.0.0.1", 1234);
}

void test_init() {
        paso = 0;
        test = 0;
        initMenasjes();
        initExpexted();
}

appMsg* test_rdt_rcv(int socket) {
        appMsg* ret = NULL;
        if (paso < TOPE) {
                sleep(1);
                ret = mensajes[paso];
                std::cout << "test_rdt_rcv: " << mensajes[paso]->mensaje << std::endl;
                paso++;
        }
        else
                sleep(300000);
        return ret;
}

void testPaso(char * msg, char * dest, int puerto) {
        bool ok = true;
        std::cout << "TEST: " << test << std::endl;
        if (expected[test] != NULL) {
                ok = assertMensaje(msg, dest, puerto, expected[test]);
        }

        if (ok) {
                std::cout << "TEST OK" << std::endl;
        }
        else {
                std::cout << "TEST ERROR" << std::endl;
        }
        test++;
}

void test_rdt_send(int soc, appMsg* msg, char* dest, int puerto) {
        sleep(2);
        testPaso(msg-> mensaje, dest, puerto);
}

void test_rdt_send_broadcast(int soc, appMsg* msg, char* dest, int puerto) {
        sleep(2);
        testPaso(msg-> mensaje, dest, puerto);
}
