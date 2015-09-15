#!/bin/sh
if test $# -ne 1
then
  echo 'Uso: ejercicio2-2.sh host1'
  exit
fi

host=$1
ispsFile="isps.txt"

trace=$(traceroute $host | sed '1d')

hostEncontrado=0

while IFS= read -r line; do
	hostName=$(echo $line | cut -d',' -f1)
	hostRegex=$(echo $line | cut -d',' -f2)
	traceRet=$(echo $trace | grep "$hostRegex")
	if [ ! -z "$traceRet" ]; then
		echo "El traceroute pasa por un host del ISP $hostName"
		hostEncontrado=1
	fi

done < "$ispsFile"

if [ $hostEncontrado -eq 0 ]; then
	echo "No se ha encontrado ningun ISP del archivo $ispsFile"
	echo "El traceroute fue:"
	echo "$trace"
fi
