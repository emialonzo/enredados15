CC = g++
FLAGS = -lpthread
IP = ifconfig wlan0 | grep 'inet:' | cut -d: -f2 | awk '{print $1}'



compilar: .servidor .cliente
	$(info Se han compilado el servidor y el cliente ✓)

.servidor: Servidor.cpp rdt.cpp mensaje.cpp constantes.h rdt_test.cpp rdt_test.h
	$(CC) Servidor.cpp rdt.cpp constantes.h  rdt_test.cpp mensaje.cpp $(FLAGS) -o .servidor

.cliente: Cliente.cpp rdt.cpp mensaje.cpp
		$(CC) Cliente.cpp rdt.cpp  mensaje.cpp  $(FLAGS) -o .cliente

ejecutarServidor: .servidor
	./.servidor

ejecutarCliente: .cliente
		./.cliente

# obtenerIp:
# 	echo IP = ifconfig wlan0 | grep 'inet:' | cut -d: -f2 | awk '{print $1}'

# mostrarIp:
# 	echo $(IP)
