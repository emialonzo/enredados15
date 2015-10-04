#include "rdt_test.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h> //sleep


#define TOPE 8

appMsg* mensajes[TOPE];

int paso;

appMsg* armarMensaje(const char* ip, const char* mensaje) {
        appMsg* ret = new appMsg;

        strcpy(ret->mensaje, mensaje);
        strcpy(ret->source_ip, ip);
        return ret;
}

void test_init() {
        paso = 0;
        mensajes[0] = armarMensaje("127.0.0.1", "LOGIN Tribilin");
        mensajes[1] = armarMensaje("127.0.0.1", "GET_CONNECTED");
        mensajes[2] = armarMensaje("127.0.0.1", "MESSAGE Hola como va?");
        mensajes[3] = armarMensaje("222.36.1.45", "LOGIN Pep");
        mensajes[4] = armarMensaje("222.36.1.45", "GET_CONNECTED");
        mensajes[5] = armarMensaje("222.36.1.45", "PRIVATE_MESSAGE Tribilin Esto es peñarol");
        mensajes[6] = armarMensaje("222.36.1.45", "LOGOUT");
        mensajes[7] = armarMensaje("127.0.0.1", "GET_CONNECTED");
}

appMsg* test_rdt_rcv() {
        appMsg* ret = NULL;
        if (paso < TOPE) {
                sleep(2);
                ret = mensajes[paso];
                std::cout << "test_rdt_rcv: " << mensajes[paso]->mensaje << std::endl;
                paso++;
        }
        else
                sleep(300000);
        return ret;
}

void test_rdt_send(appMsg* msg, char* dest) {
        std::cout << "test_rdt_send.msg: " << msg->mensaje << std::endl;
        std::cout << "test_rdt_send.dest: " << dest << std::endl;
}

void test_rdt_send_broadcast(appMsg* msg, char* dest) {
        std::cout << "test_rdt_send_broadcast:msg:" << msg->mensaje << std::endl;
        std::cout << "test_rdt_send_broadcast:dest: " << dest << std::endl;
}
