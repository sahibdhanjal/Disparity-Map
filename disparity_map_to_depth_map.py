import cv2
import numpy as np
from disparity_map import (calculate_disparity_inbuilt,
                           calculate_disparity_iterative,
                           calculate_disparity_matrix)
from matplotlib import pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D


def plot_depth(depth):
    """plots the depth map in 3D

    Arguments:
        depth {numpy.ndarray} -- the depth map
    """
    xx, yy = np.meshgrid(np.linspace(
        0, 1, depth.shape[0]), np.linspace(0, 1, depth.shape[1]))
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    surf = ax.plot_surface(xx, yy, depth.T, cmap=cm.coolwarm,
                           linewidth=0, antialiased=False)
    plt.show()


def calculate_depth_map(disparity, distance_cameras, depth_unit, fx):
    """calculates the depth map given a disparity map

    Arguments:
        disparity {numpy.ndarray} -- disparity map of the 2 stereo images
        distance_cameras {float} -- the distance between the 2 stereo cameras
        depth_unit {float} -- the unit of depth in meters
        fx {float} -- the focal length of the camera
    """
    depth = np.zeros(disparity.shape).astype(float)
    depth[disparity > 0] = (fx * distance_cameras) / \
        (depth_unit * disparity[disparity > 0])
    return depth


if __name__ == '__main__':
    left = '/home/sahib/src/photogrammetry/images/t0_left.png'
    right = '/home/sahib/src/photogrammetry/images/t0_right.png'
    disparity_map = calculate_disparity_matrix(left, right)
    plt.imshow(disparity_map, 'gray')
    plt.show()
    depth = calculate_depth_map(disparity_map, 54.8, 0.001, 942.8)
    plot_depth(depth)
