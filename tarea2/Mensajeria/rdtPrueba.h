/*
 * File:   rdt.h
 * Author: emi
 *
 * Created on 19 de septiembre de 2015, 04:00 PM
 */

#ifndef RDTPRUEBA_H
#define	RDTPRUEBA_H

#include "constantes.h"

typedef struct {
  char from[MAX_IP_LENGTH];
  bool multicast;
  bool esAck;
  int seq;
  char mensaje[MAX_RDT_MSJ];
} rdtMsj;



void iniRdt();
char* recibir();

void iniServer();
void sendMulticast(char* mensaje); //habria que pasarle lista de conectados, como desacoplamos las capas?
void rdt_send(char* datos);

#endif	/* RDT_H */
