#ifndef RDT_TEST_H
#define	RDT_TEST_H

#include "constantes.h"

typedef struct {
        char source_ip[12];
        int source_port;
        char mensaje[MAX_LARGO_MENSAJE];
} appMsg;

void test_init();

int crearSocket(int puerto, bool multicast);

appMsg* test_rdt_rcv(int socket);

void test_rdt_send(int socket, appMsg *, char* dest, int puerto);

void test_rdt_send_broadcast(int socket, appMsg *, char* dest, int puerto);

#endif
