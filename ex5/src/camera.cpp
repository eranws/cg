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
	const int m = img.getWidth();
	const int n = img.getHeight();
	const float aspectRatio = float(n)/m;

	Vector3d centerRay = _coi - _position; //Z
	double centerRayLength = centerRay.length();

	Vector3d verticalDir = _up.normalized();
	Vector3d horizontalDir = OpenMesh::cross(centerRay, _up).normalize();

	verticalDir = -verticalDir;


	for (size_t i = row_start; i < row_start + number_of_rows; i++)
	{
		double yr = (float(i) / n) * 2 - 1;  // -1 < yr < 1
		yr *= centerRayLength * tan(_fov_h) * aspectRatio;  // sceneBottom < yr < sceneTop

		for (int j = 0; j < m; j++)
		{
			double xr = (float(j) / m) * 2 - 1;  // -1 < xr < 1
			xr *= centerRayLength * tan(_fov_h); // sceneLeft < xr < sceneRight

			Point3d dir = centerRay + horizontalDir * xr + verticalDir * yr;

			Ray r(_position, dir);
			//r.normalize() called on ctor
			
			Color3d color = scene.trace_ray(r, 1.0);
			
			//TODO: optimize double->char conversion, and img pixel access (see BImage.h)
			color *= 255;

			img(j, i) = Bpixel(color[0], color[1], color[2]);

		}
	}


}

void Camera::setSamplesPerPixel(size_t samples_per_pixel)
{
	_samples_per_pixel = samples_per_pixel;
}
