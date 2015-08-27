#!/usr/bin/python
import subprocess
import re
import sys

## Funciones
def max_ping(output):
    patron = re.compile('(?P<info>.*)=(\s)(?P<min>[^/]+)/(?P<promedio>[^/]+)/(?P<max>[^/]+)/([^/]+)')
    matcher = patron.search(output)
    return matcher.group('max')

def dic_ping(output):
    patron = re.compile('(?P<info>.*)=(\s)(?P<min>[^/]+)/(?P<promedio>[^/]+)/(?P<max>[^/]+)/([^/]+)')
    matcher = patron.search(output)
    return {'min':matcher.group('min'),'promedio':matcher.group('promedio'),'max':matcher.group('max')}

def ejecutarPing(host, pingCount="1"):
    cmdping = "ping -c " + pingCount+"  " + host  + " "
    output = None
    try:
        output = subprocess.check_output(cmdping, shell=True)
        # output = subprocess.check_output(["smartctl", "-A", "/dev/sda2"])
    except CalledProcessError as e:
        output = e.output
    print "Salida:::" + output
    patron = re.compile('(?P<info>.*)=(\s)(?P<min>[^/]+)/(?P<promedio>[^/]+)/(?P<max>[^/]+)/([^/]+)')
    matcher = patron.search(output)
    print "Max:::" + max_ping(output)
    print "Max:::" + dic_ping(output)['min']
    return output

def errorPing(output):
    patron = re.compile('(ping: unknown host(?P<desconocido>.*)|From (?P<inalcanzable>.*) icmp(.*)Destination Host Unreachable)')
    matcher = patron.search(output)
    if(matcher):
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
(desconocido,inalcanzable)=errorPing(salida)
if desconocido or inalcanzable:
    print 'error con (' + desconocido + ',' + inalcanzable + ')'

print "------"
ejecutarPing("google.com", pingCount)
print "------"

# cmdping = "ping -c 1  google.com | grep max"
#
# output = subprocess.check_output(cmdping, shell=True)
# print output
