#!/bin/bash

# trap ctrl-c and call ctrl_c()
trap ctrl_c INT

function ctrl_c() {
  echo ""
  echo "** Trapped CTRL-C"
  echo $a
  exit 1
}
function execute_ping() {
  echo "ping $1"
  salida="$(ping $1 -c 1)"

  echo "Error Inalcanzable?"  #" $(echo $salida | grep Unreachable)"
  if [ -z $(echo $salida | grep Unreachable)]
  then
    echo "OK!!"
  else
    echo "Error Inalcanzable!!!"
  fi

echo ":)"
  IFS='/'
  array=( $salida )
  printf '%s\n' "${my_array[@]}"


  echo "Error TimeOut?  " #"$(echo $salida | grep max)"
  retorno="->$(echo $salida | grep max | cut -d'/' -f6)"
  com=`echo $salida | cut -d'/' -f6`
  echo "#$retorno"
  if [ -z $com ]
  then
    echo "Error Timeout!!!"
  else
    echo "OK!!"
  fi

  retorno="$(echo $salida | grep max | cut -d'/' -f6)"
  echo "El promedio de ping a $1 es $retorno"
}

# execute_ping $1
# echo "---->$salida"
# execute_ping $2
# echo "---->$salida" | grep max

a=0
while true
do
  echo "*************************"
  execute_ping $1
  echo "el valor de retorno es: $retorno"
  echo "*************************"
  sleep 6
done

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

while
do
  #statements
done


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
