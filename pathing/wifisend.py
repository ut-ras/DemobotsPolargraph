#!python3

import sys
import serial
from xml.dom import minidom
from svg.path import parse_path
import requests

<<<<<<< HEAD
POLARGRAPH_IP = "http://172.20.10.2/";
=======
>>>>>>> 4d80edac75cf9d4ddd30f2812395b80872de00fc

filename = sys.argv[1]
svg_file = open(filename)
svg_string = svg_file.read()

output = "longhorn.polar"
outputfile=open(output, 'w')

svg_dom = minidom.parseString(svg_string)
path_strings = [path.getAttribute('d') for path in svg_dom.getElementsByTagName('path')]

<<<<<<< HEAD
try:
    ser =  serial.Serial(port, 9600)
    print('Serial connected to port {}.'.format(port))
except:
    print('Serial error! Exiting.')
    sys.exit(0)

code = '{ "array": ['

=======
>>>>>>> 4d80edac75cf9d4ddd30f2812395b80872de00fc
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
<<<<<<< HEAD
        code += '"({},{})", '.format(x[index], y[index])
        #outputfile.write(code)
        #print("Writing '{}'.".format(code))
    code = code[:-1] + "] }"

    try:
        #data = str.encode(code)
        print(code)
        s = requests.post(url = POLARGRAPH_IP, data = code)
        print("Received '{}' in response; continuing.".format(s))
    except:
        print("Serial error!")
=======
        code = '({},{})'.format(x[index], y[index])
        outputfile.write(code)
        print("Writing '{}'.".format(code))
        #try:
        data = str.encode(code)
        r = requests.post(url = POLARGRAPH_IP, data = data)
        print("Received '{}' in response; continuing.".format(r))
        #except:
        #    print("HTTP error!")
>>>>>>> 4d80edac75cf9d4ddd30f2812395b80872de00fc

    print('Complete.')

    ser.write(str.encode('(0,0)'))
    s = ser.read()

    plot(x,y);show()
    #embed()
outputfile.close()
ser.close()
