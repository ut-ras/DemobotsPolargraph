#!python3

"""
USAGE: ./pathify.py inputfile
       ./pathify.py inputfile outputfile
DESCRIPTION:
    This program takes the name of an svg file as an argument. This svg file is
    converted into a single path, which is then scaled (preserving aspect
    ratio) to fill a hardcoded bounding box size. Finally, the points of this
    path are written to standard output, unless an output file name is given,
    in which case the points will be written to the output file instead.
    If setting SERIAL is true, points will be written to a given serial port.
PURPOSE:
    This program is designed to generate simple paths for a polargraph robot.
"""

from __future__ import print_function
import sys
import os.path
import serial
from xml.dom import minidom
from svg.path import parse_path
from IPython import embed
from matplotlib.pyplot import *
from math import *
style.use('dark_background')

"""
USER SETTINGS
    The output is scaled preserving aspect-ratio to [MAX_WIDTH,MAX_HEIGHT].
    MAX_WIDTH  : maximum width of output
    MAX_HEIGHT : maximum height of output
    MAX_LENGTH : maximum distance between points
    ROUND_TO_INTEGER : whether to round output values to the nearest integer
    SERIAL : whether to output values to serial port
    SERIAL_PORT : port to attempt to write values to
"""
MAX_WIDTH = 300
MAX_HEIGHT = 300
MAX_LENGTH = 1
ROUND_TO_INTEGER = True
SERIAL = False
SERIAL_PORT = '/dev/ttyUSB0'

# connect to serial port
ser = None
if SERIAL:
    ser = serial.Serial('/dev/ttyUSB0', 9000)
    print("Serial connected.")

# print to standard error
def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

# get name of file to open
if len(sys.argv) < 2:
    eprint('No input file; stopping.')
filename = sys.argv[1]
outfilename = ''
write_out = False
if len(sys.argv) >= 3:
    outfilename = sys.argv[2]
    write_out = True

# open file
if not os.path.isfile(filename):
    eprint('Input file does not exist; stopping.')
input_file = open(filename)
output_file = None
if write_out:
    output_file = open(outfilename, 'w')

# read stuff from file
svg_dom = minidom.parseString(input_file.read())
path_strings = [path.getAttribute('d') for path in svg_dom.getElementsByTagName('path')]

# utility function
def mround(num):
    if ROUND_TO_INTEGER:
        return round(num)
    return(num)

# iterate through paths in svg
#for path_string in path_strings:
path_string = path_strings[0]
if True:

    # QUICK POINT EXTRACTION

    # extract xy points from path
    path_data = parse_path(path_string)
    x = []
    y = []
    for line in path_data:
        start = (line.start.real, line.start.imag)
        end = (line.end.real, line.end.imag)
        x.append(line.start.real)
        x.append(line.end.real)
        y.append(line.start.imag)
        y.append(line.end.imag)

    # scale points
    xmin = min(x)
    xmax = max(x)
    ymin = min(y)
    ymax = max(y)
    shift_x = -(xmax+xmin)/2 # will center x by shifting by this amount
    shift_y = -(ymax+ymin)/2 # will center y by shifting by this amount
    scale_x = xmax-xmin
    scale_y = ymax-ymin
    if scale_x/MAX_WIDTH > scale_y/MAX_HEIGHT:
        scale_x = MAX_WIDTH/scale_x
        scale_y = scale_x
    else:
        scale_y = MAX_HEIGHT/scale_y
        scale_x = scale_y
    for index, item in enumerate(x):
        x[index] = round((x[index]+shift_x)*scale_x)
        y[index] = round((y[index]+shift_y)*-scale_y) # flip y axis

    # So, now we have a list of xy points, but can do better.

    # CURVE EXTRACTION

    x = []
    y = []
    for line in path_data:

        # first, find 
        num_segments = 1
        error = MAX_LENGTH * 2
        start = (line.start.real*scale_x, line.start.imag*-scale_y)
        while error > MAX_LENGTH and num_segments < 1000:
            num_segments += 1
            second = (line.point(1/num_segments).real, line.point(1/num_segments).imag)
            error = sqrt((start[0]-second[0])**2+(start[1]-second[1])**2)

        for i in range(num_segments+1):
            point = line.point(1/num_segments*i)
            x.append(((point.real+shift_x)*scale_x))
            y.append(((point.imag+shift_y)*scale_y))

    # scale again to ensure perfect fit; will only scale down
    xmin = min(x)
    xmax = max(x)
    ymin = min(y)
    ymax = max(y)
    shift_x = -(xmax+xmin)/2 # will center x by shifting by this amount
    shift_y = -(ymax+ymin)/2 # will center y by shifting by this amount
    scale_x = xmax-xmin
    scale_y = ymax-ymin
    if scale_x/MAX_WIDTH > scale_y/MAX_HEIGHT:
        scale_x = MAX_WIDTH/scale_x
        scale_y = scale_x
    else:
        scale_y = MAX_HEIGHT/scale_y
        scale_x = scale_y
    for index, item in enumerate(x):
        x[index] = mround((x[index]+shift_x)*scale_x)
        y[index] = mround((y[index]+shift_y)*-scale_y) # flip y axis

    # remove duplicates
    i = 0
    while i < len(x)-1:
        if x[i] == x[i+1] and y[i] == y[i+1]:
            del x[i+1]
            del y[i+1]
        else:
            i += 1

    # print to standard output
    for index, item in enumerate(x):
        code = '({},{})'.format(x[index],y[index])
        if write_out:
            output_file.write(code)
        elif SERIAL:
            print('Writing "{}"'.format(code))
            ser.write(str.encode(code))
            print('\tWaiting for response...')
            s = ser.read()
            print('\tReceived {}.'.format(s))
        else:
            print(code)

    if SERIAL:
        ser.write(str.encode('(0,0)'))
        s = ser.read()

    # plot points
    plot(x,y)
    gca().set_aspect('equal', adjustable='box')
    title(filename)

if write_out:
    output_file.close()
    print('Total points: {}'.format(len(x)))

# plot graph
if not (SERIAL or write_out):
    show()
