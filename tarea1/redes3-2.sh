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
  error_msg 'Uso: ejercicio3-2.sh host'
  exit
fi
hostUno=$1

#ejecuto dig
salida=`dig $hostUno ns `
salida2=`echo "$salida" | grep ";; flags"`
# obtengo la canitdad de respuestas enviadas por la consulta de tipo NS
respuesta=`echo "$salida2" | cut -d',' -f2 | cut -d':' -f2`
if test $respuesta -eq 0
then
	# como no hay respuesta, debo buscarla en el dominio
	notice_msg no hay respuesta directa, consultamos al dominio
	domain=`echo "$salida" | grep SOA | cut -d'	' -f1`
	done_msg Nombre de dominio $domain
	dig $domain ns +short
else
	#imprimo todas las respuestas obtenidas
	done_msg Hay $respuesta servidorres autoritativos
	echo "----------"
	done_msg "`dig $hostUno ns +short`"
fi
