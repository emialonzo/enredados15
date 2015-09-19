# Tarea 2

# Para compilar CLIENTE!!!!!
 agregar bandera `-lpthread`

 * a – cantidad de clientes conectados -- clientes logueados
 * s – cantidad de mensajes enviados -- lista de mensajes
 * d – cantidad de conexiones totales
 * f – tiempo (wall time) de ejecución -- guardamos tiempo al inicio y lego consultamos

## Comandos de cliente al servidor
 * **LOGIN** _usuario_ _CR_
     > Crea
 * **LOGOUT** _CR_
 * **GET_CONNECTED** _CR_
 * **MESSAGE**  _msg_ _CR_
 * **PRIVATE_MESSAGE** _receptor_ _msg_ _CR_

## Comandos del servidor al cliente:
 * **RELAYED_MESSAGE**  _emisor_ _msg_ _CR_
 * **PRIVATE_MESSAGE**  _emisor_ _msg_ _CR_
 * **CONNECTED**  _usr1_ [|_usr2_...] _CR_
 * **GOODBYE** _CR_

 • Se quiere implementar un protocolo de transferencia de datos confiable restringido a una red donde todos
 los nodos son alcanzables por paquetes de broadcast con las siguientes características:
 • debe poder transferir datos simultáneamente de un computador a todos los computadores que estén en la
 red.
 • en cada momento, la capa de aplicación debe haber recibido los datos de los mismos n paquetes,
 pudiendo diferir solo en los datos incluidos en el paquete n+1.
 • debe recuperarse de fallos de transmisión y eliminar duplicados.
 • no tendrá control de congestión.
 • no tendrá control de flujo.

* **rdt_send**_(datos)_ recibe datos desde la capa superior.
* **udt_send_broadcast**(pkt), le entrega el paquete pkt a la capa inferior para ser transmitido a todos los computadores de la red.
* **rdt_rcv**(pkt), recibe los paquetes desde la capa inferior.
* **deliver_data**(datos), entrega datos a la capa superior.
* **make_pkt**(numeroDeSecuencia, datos, checksum, origen)
* **extract_data**(pkt, datos), extrae los datos de un paquete.
* **extract_orig**(pkt, ip), extrae la dirección de origen del paquete.
* **corrupt**(pkt), verdadero sii pkt fue corrompido durante su transmisión.
* **isACK**(pkt, numeroDeSecuencia), verdadero sii pkt es un ACK con número de secuencia numeroDeSecuencia.
* **start_timer**
* **stop_timer**
* **timeout**

```
for(int i=0; i<MAX; i++){
	//algo
}
```
