CC = g++
FLAGS = -lpthread -g
IP = ifconfig wlan0 | grep 'inet:' | cut -d: -f2 | awk '{print $1}'

ARCHIVOS = mensaje.cpp  rdtPrueba.cpp
PUNTOH = mensaje.h  rdtPrueba.h constantes.h

PARAMCLIENTE = DEFAULT 127.0.0.1 54321



compilar: .servidor .cliente
	$(info Se han compilado el servidor y el cliente ✓)

.servidor: Servidor.cpp $(PUNTOH) $(ARCHIVOS)
	$(CC) Servidor.cpp  constantes.h $(ARCHIVOS) $(FLAGS) -o .servidor

.cliente: Cliente.cpp  $(PUNTOH) $(ARCHIVOS)
	$(CC) Cliente.cpp  constantes.h  $(ARCHIVOS) $(FLAGS) -o .cliente
	#	$(CC) Cliente.cpp rdt.cpp  mensaje.cpp rdtPrueba.cpp $(FLAGS) -o .cliente
	  # g++ Cliente.cpp rdt.cpp  mensaje.cpp rdtPrueba.cpp -lpthread -o  .cliente

ejecutarServidor: .servidor
	./.servidor

ejecutarCliente: .cliente
		./.cliente $(PARAMCLIENTE)

clear:
	rm .cliente .servidor

# obtenerIp:
# 	echo IP = ifconfig wlan0 | grep 'inet:' | cut -d: -f2 | awk '{print $1}'

# mostrarIp:
# 	echo $(IP)
