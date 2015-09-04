#!/bin/bash

# inicializo
if test $# -lt 1
then
  echo "Uso: bash $0 host1 "
  exit
fi

hostUno=$1
count=2

if test $# -eq 2
then
  count=$2
fi


# ejecucion de comandos
comando="ping -c $count $hostUno "
echo Ejecutando host 1 con el comando:
echo "   $comando"
# hostUnoInfo=`ping -c $count $hostUno`
hostUnoInfo=$(ping -c $count $hostUno)
echo Resultado ejecucion ping host 1
echo "$hostUnoInfo"
echo fin

comando="ping -n -c $count $hostUno "
echo Ejecutando host 1 con el -n comando:
echo "   $comando"
hostDosInfo=`ping -c $count $hostUno`
echo Resultado ejecucion ping host 1
echo "$hostDosInfo"
echo fin


echo "*****************************"
echo Comparando resultados
echo "$hostDosInfo" | grep max
echo "$hostUnoInfo" | grep max
echo "*****************************"
