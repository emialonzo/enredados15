#!/usr/bin/python
import subprocess
import re
import sys
import time

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
    # output = subprocess.check_output(cmdping, shell=True)
    try:
        output = subprocess.check_output(cmdping, shell= True, stderr= subprocess.STDOUT)
        print ">" + output
        # output = subprocess.check_output(["smartctl", "-A", "/dev/sda2"])
    except Exception as e:
        # //time.sleep(30)
        output = e.output
        # print "<" + output + ">"
        # if output == "":
        #     output = "ping: unknown host " + host
    # print ":::::::"
    # print output
    # print ":::::::"
    # patron = re.compile('(?P<info>.*)=(\s)(?P<min>[^/]+)/(?P<promedio>[^/]+)/(?P<max>[^/]+)/([^/]+)')
    # matcher = patron.search(output)
    print "Max:::" + max_ping(output)
    print "Min:::" + dic_ping(output)['min']
    return output


def errorPing(output):
    patron = re.compile('(ping: unknown host(?P<desconocido>.*))|(From (?P<inalcanzable>.*) icmp(.*)Destination Host Unreachable)')
    matcher = patron.search(output)
    if(matcher):
        print "HEY!! ERROR"
        return (matcher.group('desconocido'),matcher.group('inalcanzable'))
    else:
        return (None,None)


cantParam = len(sys.argv)
if(cantParam<2):
    print 'Error'
    print 'Uso: python ' + sys.argv[0] + ' host1 host2 [count]'
    exit(1)

#Traigo parametros
hostUno = sys.argv[1]
hostDos = sys.argv[2]
pingCount='3'
if cantParam>3:
    pingCount = sys.argv[3]


print "------"
salida=ejecutarPing(hostUno,pingCount)
print ":)" + salida
(desconocido,inalcanzable)=errorPing(salida)
if desconocido:
    print 'Error: Url desconocida ' + desconocido
if inalcanzable:
    print 'Error: Destino inalcanzable ' + inalcanzable
