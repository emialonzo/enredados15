## DIG para obtener un servidor autoritativo
El script realiza las siguientes acciones:
1. Ejecuta el comando dig con la flag NS para obtener nombre de servidor autoritativo
1. Se parsea la linea de flags para obtener la cantidad de respuestas
1. Si la cantidad de respuestas es 0
	1. Se busca el parámetro SOA el servidor que responde
	1. Se hace dig a ese domino con flag NS
1. Se parsea la respuesta obtenida en busca de los dominios
1. Se imprimen dominios en pantalla

## En base al algoritmo anterior
1. Se toma uno de los servidores autoritativos obtenidos
1. Se realiza `dig @servidorAutoritativo host RR` siendo RR el valor pasado por parámetro
1. se imprime el resultado

## En base al algoritmo 1
1. Se toma uno de los servidores autoritativos obtenidos
1. Se realiza `dig @servidorAutoritativo host MS`
1. Luego para cada servidor de la respuesta en el paso anterior se obtiene la ip ejecutando `dig @servidorAutoritativo hostObtenidoEnPaso2 A`
1. Finalmente teniendo la lista de ips de host de servidores de correo se chequea si la **ip** pasada por paramétro pertenece a dicha lista.
