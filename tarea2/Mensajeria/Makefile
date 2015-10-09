CC = g++
FLAGS = -lpthread -g
IP = ifconfig wlan0 | grep 'inet:' | cut -d: -f2 | awk '{print $1}'



compilar: .servidor .cliente
	$(info Se han compilado el servidor y el cliente ✓)

.servidor: Servidor.cpp  mensaje.cpp constantes.h rdtPrueba.cpp
	$(CC) Servidor.cpp  constantes.h rdtPrueba.cpp mensaje.cpp $(FLAGS) -o .servidor

.cliente: Cliente.cpp  mensaje.cpp  rdtPrueba.cpp
	$(CC) Cliente.cpp   mensaje.cpp rdtPrueba.cpp $(FLAGS) -o .cliente
	#	$(CC) Cliente.cpp rdt.cpp  mensaje.cpp rdtPrueba.cpp $(FLAGS) -o .cliente
	  # g++ Cliente.cpp rdt.cpp  mensaje.cpp rdtPrueba.cpp -lpthread -o  .cliente

ejecutarServidor: .servidor
	./.servidor

ejecutarCliente: .cliente
		./.cliente

clear:
	rm .cliente .servidor

# obtenerIp:
# 	echo IP = ifconfig wlan0 | grep 'inet:' | cut -d: -f2 | awk '{print $1}'

# mostrarIp:
# 	echo $(IP)
