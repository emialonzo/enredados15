#ifndef MENSAJE_H
#define	MENSAJE_H

#include "constantes.h"

typedef struct {
  char from[MAX_IP_LENGTH];
  bool esMulticast;
  bool esAck;
  int seq;
  char mensaje[MAX_RDT_MSJ];
} rdtMsj;

char* getFrom(rdtMsj);
bool esMulticast(rdtMsj);
bool esAck(rdtMsj);
int getSeq(rdtMsj);
bool getMensaje(rdtMsj);

#endif	/* MENSAJE_H */
