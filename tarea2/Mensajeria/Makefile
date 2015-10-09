CC = g++
FLAGS = -lpthread
IP = ifconfig wlan0 | grep 'inet:' | cut -d: -f2 | awk '{print $1}'



compilar: .servidor .cliente
	$(info Se han compilado el servidor y el cliente ✓)

.servidor: Servidor.cpp  mensaje.cpp constantes.h rdt_test.cpp rdt_test.h  rdtPrueba.cpp
	$(CC) Servidor.cpp  constantes.h rdtPrueba.cpp  rdt_test.cpp mensaje.cpp $(FLAGS) -o .servidor

.cliente: Cliente.cpp  mensaje.cpp  rdtPrueba.cpp
	$(CC) Cliente.cpp   mensaje.cpp rdtPrueba.cpp $(FLAGS) -o .cliente
	#	$(CC) Cliente.cpp rdt.cpp  mensaje.cpp rdtPrueba.cpp $(FLAGS) -o .cliente
	  # g++ Cliente.cpp rdt.cpp  mensaje.cpp rdtPrueba.cpp -lpthread -o  .cliente

ejecutarServidor: .servidor
	./.servidor

ejecutarCliente: .cliente
		./.cliente

# obtenerIp:
# 	echo IP = ifconfig wlan0 | grep 'inet:' | cut -d: -f2 | awk '{print $1}'

# mostrarIp:
# 	echo $(IP)
