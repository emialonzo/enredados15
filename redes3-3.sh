#!/bin/bash

# Luego de algunos problemas ocasionados por servidores de DNS que tenían
# información errónea, la empresa le pide crear un nuevo programa, que toma
# como parámetros el FQDN y el tipo de RR a consultar, y retorna el resultado. Lo
# interesante del programa es que deberá dar una respuesta que sea autoritativa

## Despliega mensajes con color
error_msg() { printf "\033[31m%s\033[0m\n" "$*"; }
notice_msg() { printf "\033[33m%s\033[0m\n" "$*"; }
done_msg() { printf "\033[32m%s\033[0m\n" "$*"; }
debug_msg() { printf "\033[34m%s\033[0m\n" "$*"; }
DONE_MSG=$(done_msg done)

# inicializo
if test $# -lt 2
then
  error_msg 'Uso: ejercicio3-3.sh host RR'
  exit
fi
hostUno=$1
RR=$2

#ejecuto dig
salida=`dig $hostUno ns `
salida2=`echo "$salida" | grep ";; flags"`
# obtengo la canitdad de respuestas enviadas por la consulta de tipo NS
respuesta=`echo "$salida2" | cut -d',' -f2 | cut -d':' -f2`
if test $respuesta -eq 0
then
	# como no hay respuesta, debo buscarla en el dominio
	# notice_msg no hay respuesta directa, consultamos al dominio
	domain=`echo "$salida" | grep SOA | cut -d'	' -f1`
	# done_msg Nombre de dominio $domain
	auto=`dig $domain ns +short`
else
	#imprimo todas las respuestas obtenidas
	# done_msg Hay $respuesta servidorres autoritativos
	# echo "----------"
    auto=`dig $hostUno ns +short`
fi
# notice_msg "$auto"
servidorAutoritativo=`echo "$auto" | sed -n 1p`

salida=`dig @$servidorAutoritativo $hostUno $RR +short`
debug_msg "$salida"
