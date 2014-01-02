/*
 * camera.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ilansh
 */

#include "camera.h"

Camera::Camera()
{

}

Camera::Camera(Point3d& pos, Point3d& coi, Vector3d& up, double fov,
		size_t samples_per_pixel) {
}

void Camera::render(size_t row_start, size_t number_of_rows, BImage& img,
		Scene& scene) const {
}

void Camera::setSamplesPerPixel(size_t samples_per_pixel) {
}
