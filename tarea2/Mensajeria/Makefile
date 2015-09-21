CC = g++
FLAGS = -lpthread


compilar: servidor cliente
	$(info Se han compilado el servidor y el cliente ✓)

servidor: Servidor.cpp
	$(CC) Servidor.cpp $(FLAGS) -o servidor

cliente: Cliente.cpp
		$(CC) Cliente.cpp $(FLAGS) -o cliente
