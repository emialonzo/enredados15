/*
 * File:   rdt.h
 * Author: emi
 *
 * Created on 19 de septiembre de 2015, 04:00 PM
 */

#ifndef RDTPRUEBA_H
#define	RDTPRUEBA_H

#include "constantes.h"
#include <map>
#include <string.h>
 using namespace std;

// typedef struct {
//   char from[MAX_IP_LENGTH];
//   bool multicast;
//   bool esAck;
//   int seq;
//   char mensaje[MAX_RDT_MSJ];
// } rdtMsj;

// • rdt_send(datos), recibe datos desde la capa superior.
// • udt_send_broadcast(pkt), le entrega el paquete pkt a la capa inferior para ser transmitido a todos los
// computadores de la red.
// • rdt_rcv(pkt), recibe los paquetes desde la capa inferior.
// • deliver_data(datos), entrega datos a la capa superior.
// • make_pkt(numeroDeSecuencia, datos, checksum, origen)
// • extract_data(pkt, datos), extrae los datos de un paquete.
// • extract_orig(pkt, ip), extrae la dirección de origen del paquete.
// • corrupt(pkt), verdadero sii pkt fue corrompido durante su transmisión.
// • isACK(pkt, numeroDeSecuencia), verdadero sii pkt es un ACK con número de secuencia numeroDeSecuencia



typedef map<char*, bool> TablaClienteId;

int CrearSocket(int puerto, bool multicast);
char* rdt_recibe(int soc, char*& ipEmisor, int& puertoEmisor);
void rdt_sendto(int soc, char* mensajeToSend, char* ip, int puerto);
void rdt_send_multicast(int soc, char* mensajeToSend, TablaClienteId* tablaClientes);

// void iniRdt();
// char* recibir();
// void iniServer();
// void sendMulticast(char* mensaje); //habria que pasarle lista de conectados, como desacoplamos las capas?
// void rdt_send(char* datos);

#endif	/* RDT_H */
