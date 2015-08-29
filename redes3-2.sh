#!/bin/bash

# Lo primero que le solicitan es que escriba un programa que tome como parámetro
# un nombre de dominio, y que retorne el o los servidores de nombre
# autoritativos para ese dominio

## Despliega mensajes con color
error_msg() { printf "\033[31m%s\033[0m\n" "$*"; }
notice_msg() { printf "\033[33m%s\033[0m\n" "$*"; }
done_msg() { printf "\033[32m%s\033[0m\n" "$*"; }
DONE_MSG=$(done_msg done)

# inicializo
if test $# -eq 0
then
  error_msg 'Uso: ejercicio1-2.sh host1 [count]'
  exit
fi

hostUno=$1


salida=`dig $hostUno ns `
salida2=`echo "$salida" | grep ";; flags"`
notice_msg moco "$salida"
respuesta=`echo "$salida2" | cut -d',' -f2 | cut -d':' -f2`
if test $respuesta -eq 0
then
	error_msg no hay respuesta
else
	done_msg un monton de respuestas
fi
done_msg Oinc!! $1
