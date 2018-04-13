#!python3

import sys
import serial
from xml.dom import minidom
from svg.path import parse_path
#from IPython import embed
#from matplotlib.pyplot import *
import requests

POLARGRAPH_IP = "http://192.168.43.40";

filename = sys.argv[1]
svg_file = open(filename)
svg_string = svg_file.read()

output = "longhorn.polar"
outputfile=open(output, 'w')

svg_dom = minidom.parseString(svg_string)
path_strings = [path.getAttribute('d') for path in svg_dom.getElementsByTagName('path')]

for path_string in path_strings:
    path_data = parse_path(path_string)

    x = []
    y = []

    for line in path_data:
        start = ((line.start.real), (line.start.imag))
        end = ((line.end.real), (line.end.imag))
        x.append(start[0])
        x.append(end[0])
        y.append(start[1])
        y.append(end[1])

    xmax = max(x)
    xmin = min(x)
    ymax = max(y)
    ymin = min(y)
    shift_x = -(xmax+xmin)/2
    shift_y = -(ymax+ymin)/2
    width = 300
    height = (ymax-ymin)*300/(xmax-xmin)
    scale_x = width/(xmax-xmin)
    scale_y = -height/(ymax-ymin) # flip
 
    for index, item in enumerate(x):
        x[index] = round((x[index]+shift_x)*scale_x)
        y[index] = round((y[index]+shift_y)*scale_y)
        code = '({},{})'.format(x[index], y[index])
        outputfile.write(code)
        print("Writing '{}'.".format(code))
        #try:
        data = str.encode(code)
        r = requests.post(url = POLARGRAPH_IP, data = data)
        print("Received '{}' in response; continuing.".format(r))
        #except:
        #    print("HTTP error!")

    print('Complete.')


outputfile.close()
