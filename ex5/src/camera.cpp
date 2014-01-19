/*
* camera.cpp
*
*  Created on: Jan 2, 2014
*      Author: ilansh
*/

#include "camera.h"
#include "bimage.h"
#include "ray.h"
#include "scene.h"

Camera::Camera() :
	_position(Point3d()),
	_coi(Point3d(0, 0, 1)),
	_up(Point3d(0, 1, 0)),
	_fov_h(45),
	_samples_per_pixel(1)
{

}

Camera::Camera(Point3d& pos, Point3d& coi, Vector3d& up, double fov, size_t samples_per_pixel) :
	_position(pos),
	_coi(coi),
	_up(up),
	_fov_h(fov),
	_samples_per_pixel(samples_per_pixel)
{

}

void Camera::render(size_t row_start, size_t number_of_rows, BImage& img, Scene& scene) const
{

	//these can be computed in advance, but we sticked to the given api...
	const int m = img.getWidth();
	const int n = img.getHeight();
	const float aspectRatio = float(n)/m;

	Vector3d centerRay = (_coi - _position).normalize();
	Vector3d horizontal = (centerRay % _up).normalize();
	Vector3d vertical = (centerRay % horizontal).normalize();

	Vector3d dx = horizontal * 2 * tan(_fov_h) / img.getWidth();
	Vector3d dy = vertical * 2 * aspectRatio * tan(_fov_h) / img.getHeight();


	for (size_t samples = 0; samples < _samples_per_pixel; samples++)
	{
		// if number of samples == 1: shoot a ray through the pixel center
		// else shoot few random rays
		float sx = 0.5;
		float sy = 0.5;
		if (_samples_per_pixel > 1)
		{
			sy = float(rand()) / RAND_MAX;
			sx = float(rand()) / RAND_MAX;
		}
			

		for (size_t i = row_start; i < row_start + number_of_rows; i++)
		{
			for (int j = 0; j < m; j++)
			{
				//create a ray that passes through the current pixel
				Vector3d dir = (j + sx - img.getWidth() * 0.5)*dx + (i + sy - img.getHeight() * 0.5)*dy + centerRay;
				Ray r(_position, dir);

				//trace ray
				Color3d color = scene.trace_ray(r, 1.0);

				//avoid overflow on saturated pixels
				color[0] = std::min(color[0], 1.0);
				color[1] = std::min(color[1], 1.0);
				color[2] = std::min(color[2], 1.0);

				//write on the image object
				color *= 255;
				img(j, i) = Bpixel(color[0], color[1], color[2]);

			}
		}

	}//end samples

}

void Camera::setSamplesPerPixel(size_t samples_per_pixel)
{
	_samples_per_pixel = samples_per_pixel;
}
