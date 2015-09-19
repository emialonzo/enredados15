# Tarea 2

 * a – cantidad de clientes conectados -- clientes logueados
 * s – cantidad de mensajes enviados -- lista de mensajes
 * d – cantidad de conexiones totales
 * f – tiempo (wall time) de ejecución -- guardamos tiempo al inicio y lego consultamos

## Comandos de cliente al servidor
 * LOGIN _usuario_ _CR_
 * LOGOUT _CR_
 * GET_CONNECTED _CR_
 * MESSAGE  _msg_ _CR_
 * PRIVATE_MESSAGE _receptor_ _msg_ _CR_

## Comandos del servidor al cliente:
 * RELAYED_MESSAGE <emisor> <msg><CR>
 * PRIVATE_MESSAGE <emisor> <msg><CR>
 * CONNECTED <usr1>[|<usr2>...]<CR>
 * GOODBYE<CR>

```
for(int i=0; i<MAX; i++){
	//algo
}
```
