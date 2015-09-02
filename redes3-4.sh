#!/bin/bash

# Por esta razón necesitan que usted haga un programa que dado un
# nombre de dominio y una dirección IP (que es la IP del host que se conectó y
# pretende enviar un correo usando un remitente que pertenece al dominio pasado
# como primer parámetro) retorne una respuesta booleana que diga que esa IP es
# efectivamente una dirección IP correspondiente a un servidor de correo de ese
# dominio

## Despliega mensajes con color
error_msg() { printf "\033[31m%s\033[0m\n" "$*"; }
notice_msg() { printf "\033[33m%s\033[0m\n" "$*"; }
done_msg() { printf "\033[32m%s\033[0m\n" "$*"; }
debug_msg() { printf "\033[34m%s\033[0m\n" "$*"; }
DONE_MSG=$(done_msg done)

# inicializo
if test $# -lt 2
then
  error_msg 'Uso: ejercicio3-4.sh host ip'
  exit
fi
hostUno=$1
IP=$2

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

salida=`dig @$servidorAutoritativo $hostUno mx +short`
debug_msg "$salida"
echo "--------------"
salida2=`dig @$servidorAutoritativo $hostUno mx +short | cut -d" " -f2  | xargs dig +short`
debug_msg "$salida2"


if echo "$salida2"  | grep -q $IP
then
    done_msg "eureca! :)"
else
    error_msg "no estaba :("
fi
