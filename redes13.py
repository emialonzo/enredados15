#!/usr/bin/python
import subprocess
import re
import sys
import time

DEBUG=0

## Funciones
def  max_ping(output):
    patron = re.compile('(?P<info>.*)=(\s)(?P<min>[^/]+)/(?P<promedio>[^/]+)/(?P<max>[^/]+)/([^/]+)')
    matcher = patron.search(output)
    if matcher:
        return matcher.group('max')
    else:
        return ""


def dic_ping(output):
    patron = re.compile('(?P<info>.*)=(\s)(?P<min>[^/]+)/(?P<promedio>[^/]+)/(?P<max>[^/]+)/([^/]+)')
    matcher = patron.search(output)
    if matcher:
        return {'min': matcher.group('min'),'promedio': matcher.group('promedio'),'max': matcher.group('max')}
    else:
        return {'min': "", 'promedio': "", 'max': ""}


def ejecutarPing(host, pingCount="1"):
    cmdping = "ping -c " + pingCount + "  " + host + " "
    output = None
    try:
        output = subprocess.check_output(cmdping, shell= True, stderr= subprocess.STDOUT)
        # print ">" + output
        # output = subprocess.check_output(["smartctl", "-A", "/dev/sda2"])
    except Exception as e:
        output = e.output

    if DEBUG:
        print "DEBUG::{host:" + host + ",Ping:" + output.split('\n')[-2] + "}"
    return output


def errorPing(output):
    patron = re.compile('(ping: unknown host(?P<desconocido>.*))|(From (?P<inalcanzable>.*) icmp(.*)Destination Host Unreachable)')
    matcher = patron.search(output)
    if(matcher):
        # print "HEY!! ERROR"
        return (matcher.group('desconocido'),matcher.group('inalcanzable'))
    else:
        return (None,None)

def mejorPromedioq(salida1, salida2):
    dictPing1 = dic_ping(salida1)
    dictPing2 = dic_ping(salida2)
    if float(dictPing1['promedio']) > float(dictPing2['promedio']):
        return 1
    else:
        return 2


def procesarErrores(salida):
    (desconocido,inalcanzable)=errorPing(salida)
    if desconocido:
        print 'Error: Url desconocida ' + desconocido
    if inalcanzable:
        print 'Error: Destino inalcanzable ' + inalcanzable
    dictPing = dic_ping(salida)
    if float(dic_ping(salida)['max']) > umbral:
        raise Exception('Se ha superado el umbral de tiempo de ' + str(umbral) +'ms')



######################## MAIN #####################
cantParam = len(sys.argv)
if(cantParam<2):
    print 'Error'
    print 'Uso: python ' + sys.argv[0] + ' host1 host2 [count] [umbral]'
    exit(1)

#parametros por defecto
pingCount='3'
umbral=50.00

#Traigo parametros
hostUno = sys.argv[1]
hostDos = sys.argv[2]
if cantParam>3:
    pingCount = sys.argv[3]

if cantParam>4:
    umbral = float(sys.argv[4])

if cantParam>5:
    DEBUG = int(sys.argv[5])


print " -- Comenzando el monitoreo --"
print "Hosts: " + hostUno + " y " + hostDos
print "Ping por iteracion:" + pingCount
print "Umbral: " + str(umbral) + "ms."
print "-------------------------------"


while True:
    try:
        salidaUno=ejecutarPing(hostUno,pingCount)
        procesarErrores(salidaUno)
        salidaDos=ejecutarPing(hostDos,pingCount)
        procesarErrores(salidaDos)
    except Exception as e:
        print "Error: " + e.message
        # print 'Cerrando'
        # exit(1)
