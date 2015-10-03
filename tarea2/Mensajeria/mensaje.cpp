#include "mensaje.h"

char* getFrom(rdtMsj mensaje){
  return mensaje.from;
}
bool esMulticast(rdtMsj mensaje){
  return mensaje.esMulticast;
}
bool esAck(rdtMsj mensaje){
  return mensaje.esAck;
}
int getSeq(rdtMsj mensaje){
  return mensaje.seq;
}
bool getMensaje(rdtMsj mensaje){
  return mensaje.mensaje;
}
