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


# 1
def testPainterly(im, texture, outputName, N=10000, size=50, noise=0.3):
    '''
    Using extra debug flags, outputs both the first and second
    passes ONLY of the painterly function. Similar to the handout.
    '''
    # uses debug flags as extensions to painterly
    npr.painterly(im, texture, N, size, noise, debug=True, imname=outputName)


# 2
def testForest(N=50000, size=200, noise=0.1):
    '''
    Tests NPR on a beautiful image of a forest I found
    on Google Images.

    Generates a squiggly, worm-looking image.
    Quite abstract.
    '''
    io.imwrite(npr.orientedPaint(forest, bigBrush, N, size, noise),
               str("ForestOrientedPaint"+".png"))


# 3
def testForestIntermediate(N=50000, size=200, noise=0.1):
    '''
    Verifies the intermediate passes in the painterly function.
    This works on the same forest image as before.
    '''
    npr.painterly(forest, bigBrush, N, size, noise,
                  debug=True, imname="Forest")


# 4
def testCatalina(N=50000, size=50, noise=0.2):
    '''
    Renders a painted version of Catalina.
    Refer to the macOS Catalina default dynamic wallpaper
    for this.
    '''
    io.imwrite(npr.orientedPaint(catalina, brush1, N, size, noise),
               str("CatalinaOrientedPaint"+".png"))


# 5
def testCatalinaIntermediate(N=50000, size=50, noise=0.2):
    '''
    Verifies the intermediate passes in the painterly function.
    This works on the same Catalina image as before in #4.
    '''
    npr.painterly(catalina, brush1, N, size, noise,
                  debug=True, imname="Catalina")


testPainterly(vpd, brush1, "VPD")
testForest()
testForestIntermediate()
testCatalina()
testCatalinaIntermediate()
