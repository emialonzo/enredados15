#!/bin/bash

error_msg() { printf "\033[31m%s\033[0m\n" "$*"; }
notice_msg() { printf "\033[33m%s\033[0m " "$*"; }
done_msg() { printf "\033[32m%s\033[0m\n" "$*"; }
DONE_MSG=$(done_msg done)

# inicializo
if test $# -lt 2
then
  error_msg 'Uso: ejercicio1-2.sh host1 host2 [count]'
  exit
fi

hostUno=$1
hostDos=$2
count=1

if test $# -eq 3
then
  count=$3
fi



echo "**  Iniciado script para determinar host con mejor tiempo de respuesta **"
echo "  Host1: $hostUno"
echo "  Host2: $hostDos"
echo "**********************************"


# ejecucion de comandos
comando="ping -c $count $hostUno | grep max | cut -d'/' -f6"
# echo Ejecutando host 1 con el comando:
# echo "   $comando"
hostUnoInfo=`ping -c $count $hostUno | grep max | cut -d'/' -f6`
if test ! "$hostUnoInfo"
then
    hostUnoInfo=0
fi
# echo Fin ejecucion ping host 1


comando="ping -c $count $hostDos | grep max | cut -d'/' -f6"
# echo Ejecutando host 2 con el comando:
# echo "   $comando"
hostDosInfo=`ping -c $count $hostDos | grep max | cut -d'/' -f6`
# if echo $hostDosInfo  | grep -q '[0-9]\+'
if test ! "$hostDosInfo"
then
    hostDosInfo=0
fi
# echo Fin ejecucion ping host 2
# echo "**********************************"

# Resultados
echo El RTT promedio para $hostUno es $hostUnoInfo ms
echo El RTT promedio para $hostDos es $hostDosInfo ms

echo "**********************************"
if test 1 -eq "$(echo "${hostUnoInfo} < ${hostDosInfo}" | bc)"
then
  done_msg "## $hostUno tiene el mejor tiempo de respuesta ##"
else
  done_msg "## $hostDos tiene el mejor tiempo de respuesta ##"
fi
