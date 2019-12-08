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

    # size is the product of the dimensions, so we divide by the max
    texture_scaled = helper.scaleImage(texture, float(size) / max(texture.shape))
    out_height = out.shape[0]
    out_width = out.shape[1]

    num_strokes = 0
    # generate N strokes of the brush
    while (num_strokes < N):
        # randrange intervals [0, x), end noninclusive
        y, x = rnd.randrange(0, out_height), rnd.randrange(0, out_width)
        rejection_factor = rnd.random()
        if importance[y, x][0] > rejection_factor:
            noise_factor = 1 - noise / 2 + noise * np.random.rand(3)
            color = im[y, x] * noise_factor
            brush(out, y, x, color, texture_scaled)
            # keeping increment inside ensures N strokes
            num_strokes += 1


def painterly(im, texture, N=10000, size=50, noise=01.3):
    '''First paints at a coarse scale using all 1's for importance sampling, then paints again at size/4 scale using the sharpness map for importance sampling.'''

    out = io.constantIm(im.shape[0], im.shape[1])

    # first pass
    importance_first_pass = np.ones_like(im)
    singleScalePaint(im, out, importance_first_pass, texture, size, N, noise)

    # second pass
    importance_second_pass = helper.sharpnessMap(im)
    singleScalePaint(im, out, importance_second_pass, texture, size/4, N, noise)

    return out


def computeAngles(im):
    '''Return an image that holds the angle of the smallest eigenvector of the structure tensor at each pixel. If you have a 3 channel image as input, just set all three channels to be the same value theta.'''
    angles = np.zeros(im.shape)
    # default function gives an error
    tensor = helper.computeTensor(im, sigmaG=3, factor=5, debug=False)
    # iterate through whole thing
    for y in xrange(im.shape[0]):
        for x in xrange(im.shape[1]):
            eigenvalues, eigenvectors = np.linalg.eigh(tensor[y, x])
            min_vec = eigenvectors[:, np.argmin(eigenvalues)] # indexing syntax wonky!
            theta = np.arctan2(min_vec[0], min_vec[1]) + math.pi
            angles[y, x] = theta # add pi to offset negative angles
    return angles


def singleScaleOrientedPaint(im, out, thetas, importance, texture, size, N, noise, nAngles=36):
    '''same as single scale paint but now the brush strokes will be oriented according to the angles in thetas.'''
    pass


def orientedPaint(im, texture, N=7000, size=50, noise=0.3):
    '''same as painterly but computes and uses the local orientation information to orient strokes.'''
    pass
