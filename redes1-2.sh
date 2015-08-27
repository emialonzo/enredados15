#!/bin/bash

# inicializo
if test $# -ne 2
then
  echo 'Uso: ejercicio1-2.sh host1 host2 [count]'
  exit
fi

hostUno=$1
hostDos=$2
count=1

echo "**********************************"
echo Debug info:
echo "  Host1: $hostUno"
echo "  Host2: $hostDos"
echo "  Cantidad parametros: $#"
echo "**********************************"


# ejecucion de comandos
comando="ping -c $count $hostUno | grep max | cut -d'/' -f6"
echo Ejecutando host 1 con el comando:
echo "   $comando"
hostUnoInfo=`ping -c $count $hostUno | grep max | cut -d'/' -f6`
echo Fin ejecucion ping host 1


comando="ping -c $count $hostDos | grep max | cut -d'/' -f6"
echo Ejecutando host 2 con el comando:
echo "   $comando"
hostDosInfo=`ping -c $count $hostDos | grep max | cut -d'/' -f6`
echo Fin ejecucion ping host 2

echo "**********************************"
# Resultados
echo El RTT promedio para el host 1 es $hostUnoInfo ms
echo El RTT promedio para el host 2 es $hostDosInfo ms

echo "**********************************"
if test 1 -eq "$(echo "${hostUnoInfo} < ${hostDosInfo}" | bc)"
then
  echo "## $hostUno tiene el mejor tiempo de respuesta ##"
else
  echo "## $hostDos tiene el mejor tiempo de respuesta ##"
fi
