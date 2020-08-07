"""
Reference Documents:
https://sites.google.com/site/5kk73gpu2010/assignments/stereo-vision#TOC-Update-Disparity-Map
https://www.intelrealsense.com/stereo-depth-vision-basics/
"""

import cv2
import numpy as np
from matplotlib import pyplot as plt


def preprocess_frame(path):
    """reads and pre-processes the image

    Arguments:
        path {str} -- path to image

    Returns:
        numpy.ndarray -- image
    """
    image = cv2.imread(path)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
    image = cv2.equalizeHist(image)
    return image


def calculate_disparity_iterative(left_path,
                                  right_path,
                                  num_disparities=64,
                                  block=15):
    """calculates the disparities between 2 images iteratively

    Arguments:
        left_path {str} -- path to left image
        right_path {str} -- path to right image

    Keyword Arguments:
        num_disparities {int} -- number of disparities to 
        account for (default: {64})
        block {int} -- block size of kernel or window for 
        calculating SAD (default: {15})

    Returns:
        numpy.ndarray -- the disparity map
    """
    left_image = preprocess_frame(left_path)
    right_image = preprocess_frame(right_path)
    rows, cols = left_image.shape

    # add padding the size of the block to facilitate matching on edge and corner pixels
    padding_size = int(block / 2) - 1
    left = cv2.copyMakeBorder(left_image, padding_size, padding_size,
                              padding_size, padding_size, cv2.BORDER_CONSTANT)
    right = cv2.copyMakeBorder(right_image, padding_size, padding_size,
                               padding_size, padding_size, cv2.BORDER_CONSTANT)

    # initialize 2 matrices the size of the original images
    minSAD = np.ones(left_image.shape[:2]) * float('inf')
    SAD = np.empty(left_image.shape[:2])
    disparity = np.empty(left_image.shape[:2])

    for k in range(0, num_disparities):
        translation_matrix = np.float32([[1, 0, k], [0, 1, 0]])
        shifted_image = cv2.warpAffine(right, translation_matrix,
                                       (right.shape[1], right.shape[0]))
        print('Processing K: {}'.format(k + 1))

        # calculate the disparity of every block in the padded left image
        # and the padded and shifted right image
        for row in range(block, left.shape[0] - block):
            for col in range(block, left.shape[1] - block):
                left_window = left[row - block:row + block,
                                   col - block:col + block]
                right_window = shifted_image[row - block:row + block,
                                             col - block:col + block]
                SAD[row - block, col - block] = np.sum(
                    abs(
                        left_window.astype('float32') - right_window.astype('float32')))

        for col in range(0, cols):
            for k in range(num_disparities):
                if (SAD[row, col] < minSAD[row, col]):
                    minSAD[row, col] = SAD[row, col]
                    disparity[row, col] = k

    return disparity


def calculate_disparity_matrix(left_path,
                               right_path,
                               num_disparities=64,
                               block=15):
    """calculates the disparities between 2 images using convolutions and matrix math

    Arguments:
        left_path {str} -- path to left image
        right_path {str} -- path to right image

    Keyword Arguments:
        num_disparities {int} -- number of disparities to account for (default: {64})
        block {int} -- block size of kernel or window for calculating SAD (default: {15})

    Returns:
        numpy.ndarray -- the disparity map
    """
    left_image = preprocess_frame(left_path)
    right_image = preprocess_frame(right_path)
    rows, cols = left_image.shape

    kernel = np.ones([block, block]) / block

    disparity_maps = np.zeros(
        [left_image.shape[0], left_image.shape[1], num_disparities])
    for d in range(0, num_disparities):
        # shift image
        translation_matrix = np.float32([[1, 0, d], [0, 1, 0]])
        shifted_image = cv2.warpAffine(
            right_image, translation_matrix,
            (right_image.shape[1], right_image.shape[0]))
        # calculate squared differences
        SAD = abs(np.float32(left_image) - np.float32(shifted_image))
        # convolve with kernel and find SAD at each point
        filtered_image = cv2.filter2D(SAD, -1, kernel)
        disparity_maps[:, :, d] = filtered_image

    disparity = np.argmin(disparity_maps, axis=2)
    disparity = np.uint8(disparity * 255 / num_disparities)
    return cv2.equalizeHist(disparity)


def calculate_disparity_inbuilt(left_path,
                                right_path,
                                num_disparities=64,
                                block=15):
    """calculates the disparities between 2 images using the inbuilt open cv algorithm

    Arguments:
        left_path {str} -- path to left image
        right_path {str} -- path to right image

    Keyword Arguments:
        num_disparities {int} -- number of disparities to account for (default: {64})
        block {int} -- block size of kernel or window for calculating SAD (default: {15})

    Returns:
        numpy.ndarray -- the disparity map
    """
    left_image = preprocess_frame(left_path)
    right_image = preprocess_frame(right_path)
    rows, cols = left_image.shape
    stereo = cv2.StereoBM_create(numDisparities=num_disparities,
                                 blockSize=block)
    return stereo.compute(left_image, right_image)


def plot_disparity(disparity):
    """Plots the disparity map in grayscale

    Arguments:
        disparity {numpy.ndarray} -- the disparity map
    """
    plt.imshow(disparity, 'gray')
    plt.show()
