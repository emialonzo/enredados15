/*
 * File:   rdt.h
 * Author: emi
 *
 * Created on 19 de septiembre de 2015, 04:00 PM
 */

#ifndef RDT_H
#define	RDT_H

typedef struct {
  char* from;
  bool multicast;
  bool ack;
  char* mensaje;
} rdtMsj;



void iniRdt();
char* recibir();

void iniServer();
void sendMulticast(char* mensaje); //habria que pasarle lista de conectados, como desacoplamos las capas?
void rdt_send(char* datos);

#endif	/* RDT_H */
