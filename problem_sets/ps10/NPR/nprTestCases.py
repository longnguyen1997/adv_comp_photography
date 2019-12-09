# LONG NGUYEN
# 6.865 - COMPUTATIONAL PHOTOGRAPHY
# FALL 2019
# ASSIGNMENT 10

import imageIO as io
import npr
import glob
import random as rnd
import numpy as np
import nprHelper as nprh
import math
from time import time

'''
nprScript.py contains some basic test cases.

This script contains more test cases for functionality.
Refer to 6.865 Fall 2019 for more information on
Assignment 10 (free exploration).

ASSIGNMENT:
At least 5 well-documented test cases testing intermediate parts of your algorithm.
This should be similar to the test cases we have provided you on previous
assignments. Include your test cases in the a10main.cpp file. Include figures
in the write-up if appropriate.
'''

# Brushes
brush1 = io.imread('brush.png')
longBrush = io.imread('longBrush.png')
bigBrush = io.imread('longBrush2.png')
roundIm = io.imread('round.png')

# Provided images
liz = io.imread('liz.png')
china = io.imread('china.png')
vpd = io.imread('villeperdue.png')

# My images
forest = io.imread('forest.png')
catalina = io.imread('catalina.png')


'''
Print out, useful statistics if appropriate,
e.g. running times, averages, etc.
'''
def timeprofile(func):
    def wrapper(im, *args, **kwds):
        print '- - - - - - - - - - - - - - - - - - - - - - -'
        print 'Operating on image of size ' + str(im.shape) + '...'
        before = time()
        func(im, *args, **kwds)
        print '  > time to completion: %fs' % (time() - before)
    return wrapper


# 1
@timeprofile
def testPainterly(im, texture, outputName, N=10000, size=50, noise=0.3):
    '''
    Using extra debug flags, outputs both the first and second
    passes ONLY of the painterly function. Similar to the handout.
    '''
    print 'TEST 1: painterly on provided VPD image'
    # uses debug flags as extensions to painterly
    npr.painterly(im, texture, N, size, noise, debug=True, imname=outputName)


# 2
@timeprofile
def testForest(im, N=50000, size=25, noise=0.4):
    '''
    Tests NPR on a beautiful image of a forest I found
    on Google Images.

    Generates a squiggly, worm-looking image.
    Quite abstract.
    '''
    print 'TEST 2: oriented brushing on beautiful forest image'
    io.imwrite(npr.orientedPaint(im, bigBrush, N, size, noise),
               str("ForestOrientedPaint"+".png"))


# 3
@timeprofile
def testForestIntermediate(im, N=50000, size=25, noise=0.4):
    '''
    Verifies the intermediate passes in the painterly function.
    This works on the same forest image as before.
    '''
    print 'TEST 3: painterly on forest (outputting intermediate passes)'
    npr.painterly(im, bigBrush, N, size, noise,
                  debug=True, imname="Forest")


# 4
@timeprofile
def testCatalina(im, N=50000, size=50, noise=0.2):
    '''
    Renders a painted version of Catalina.
    Refer to the macOS Catalina default dynamic wallpaper
    for this.
    '''
    print 'TEST 4: oriented brushing on macOS Catalina wallpaper'
    io.imwrite(npr.orientedPaint(im, brush1, N, size, noise),
               str("CatalinaOrientedPaint"+".png"))


# 5
@timeprofile
def testCatalinaIntermediate(im, N=50000, size=50, noise=0.2):
    '''
    Verifies the intermediate passes in the painterly function.
    This works on the same Catalina image as before in #4.
    '''
    print 'TEST 5: painterly on Catalina wallpaper (outputting intermediate passes)'
    npr.painterly(im, brush1, N, size, noise,
                  debug=True, imname="Catalina")


''' MAIN TEST CASES HERE '''
# testPainterly(vpd, brush1, "VPD")
# testForest(forest)
# testForestIntermediate(forest)
# testCatalina(catalina)
# testCatalinaIntermediate(catalina)

''' FURTHER EXTENSION FOR 6.865 '''
# npr.paintAndOutline(catalina, brush1, debug=True)
npr.paintAndOutline(forest, brush1, debug=True, size=25, N=50000, noise=0.4)
