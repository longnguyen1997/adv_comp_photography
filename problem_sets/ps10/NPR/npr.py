# npr.py
import imageIO as io
#import a2
import numpy as np
import png
import scipy as sp
from scipy import signal
from scipy import ndimage
import random as rnd
import nprHelper as helper
import math


def brush(out, y, x, color, texture):
    '''out: the image to draw to. y,x: where to draw in out. color: the color of the stroke. texture: the texture of the stroke.'''

    # color: 3-array
    # texture: opacity

    out_height = out.shape[0]
    out_width = out.shape[1]
    texture_height = texture.shape[0]
    texture_width = texture.shape[1]

    closer_than_half_away_from_boundary = (
        not texture_width / 2 <= x <= out_width - texture_width / 2 - 1
        or
        not texture_height / 2 <= y <= out_height - texture_height / 2 - 1
    )

    if closer_than_half_away_from_boundary:
        return

    color_image = texture.copy()
    # flood color image with the exact same color
    color_image[:, :] = color

    x_offset_left = x_offset_right = texture_width / 2
    y_offset_left = y_offset_right = texture_height / 2

    # ensure that we're covering the entire dimensions of the texture
    if (y_offset_right * 2 != texture_height):
        y_offset_right += 1
    if (x_offset_right * 2 != texture_width):
        x_offset_right += 1

    # write a linear combination to the output
    out[y - y_offset_left : y + y_offset_right, x - x_offset_left : x + x_offset_right] = (
        out[y - y_offset_left : y + y_offset_right, x - x_offset_left : x + x_offset_right] *
        (1 - texture) + color_image * (texture)
    )


def singleScalePaint(im, out, importance, texture, size=10, N=1000, noise=0.3):
    '''Paints with all brushed at the same scale using importance sampling.'''

    k = float(size) / max(texture.shape)
    texture_scaled = helper.scaleImage(texture, k)
    outH = out.shape[0]
    outW = out.shape[1]

    strokes = 0
    while (strokes < N):
        (y, x) = (rnd.randrange(0, outH), rnd.randrange(0, outW))
        u = np.random.rand()

        if importance[y,x][0] > u:

            col = im[y, x]
            modNoise = 1-noise/2+noise*np.random.rand(3)
            col *= modNoise
            brush(out, y, x, col, texture_scaled)
            strokes += 1


    pass


def painterly(im, texture, N=10000, size=50, noise=0.3):
    '''First paints at a coarse scale using all 1's for importance sampling, then paints again at size/4 scale using the sharpness map for importance sampling.'''
    pass


def computeAngles(im):
    '''Return an image that holds the angle of the smallest eigenvector of the structure tensor at each pixel. If you have a 3 channel image as input, just set all three channels to be the same value theta.'''
    pass


def singleScaleOrientedPaint(im, out, thetas, importance, texture, size, N, noise, nAngles=36):
    '''same as single scale paint but now the brush strokes will be oriented according to the angles in thetas.'''
    pass


def orientedPaint(im, texture, N=7000, size=50, noise=0.3):
    '''same as painterly but computes and uses the local orientation information to orient strokes.'''
    pass
