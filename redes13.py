#!/usr/bin/python
import subprocess
import re

pingCount="3"

# strMatch="La casa amarilla La pinta lindo"
# print "probando expresiones regulares con python: "+strMatch
# print re.match(r'(La)', strMatch, flags=0).group()
# exit(0)

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
    output = subprocess.check_output(cmdping, shell=True)
    print "Salida:::" + output
    patron = re.compile('(?P<info>.*)=(\s)(?P<min>[^/]+)/(?P<promedio>[^/]+)/(?P<max>[^/]+)/([^/]+)')
    matcher = patron.search(output)
    print "Max:::" + max_ping(output)
    print "Max:::" + dic_ping(output)['min']


print "------"
ejecutarPing("google.com", pingCount)
print "------"

cmdping = "ping -c 1  google.com | grep max"

output = subprocess.check_output(cmdping, shell=True)
print output
