#ifndef RDT_TEST_H
#define	RDT_TEST_H

#include "constantes.h"

typedef struct {
        char source_ip[12];
        char mensaje[MAX_LARGO_MENSAJE];
} appMsg;

void test_init();

appMsg* test_rdt_rcv();

void test_rdt_send(appMsg *, char* dest);

void test_rdt_send_broadcast(appMsg *, char* dest);

#endif
