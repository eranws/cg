/*
 * Ellipsoid.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ilansh
 */

#include "Lens.h"


Lens::Lens(Point3d center, double radius, double width) : _center(center), _r(radius), _w(width)
{
	double sphereRadius = (_w * _w + _r * _r) / (2 * _w);
	Vector3d spherePosition(0,  _r - _w, 0);

	_sphere1 = Sphere(_center + spherePosition, sphereRadius);
	_sphere2 = Sphere(_center - spherePosition, sphereRadius);
}

Lens::~Lens()
{
}


int Lens::intersect(Ray& ray, double tMax, double& t, Point3d& P, Vector3d& N,
		Color3d& texColor) const
{
	int retVal = 0;

	double t11, t12;
	if (!_sphere1.getRoots(ray, &t11, &t12))
		return 0;
	
	double t21, t22;
	if (!_sphere2.getRoots(ray, &t21, &t22))
		return 0;


	const Sphere* sp;

	//TODO: check if inside
	if (t11 < t21 && t21 < t22) //ray hits sphere1 first (even if ray is inside...)
    {
		if (EPS < t21)
		{
	        retVal = 1;
			t = t21;
			sp = &_sphere2;
		}
		else
		{

		}



    }
	else if (t22 < t12 && t12 < t11)
	{
		retVal = 1;
        t = t12;
		sp = &_sphere1;
    }


	if (retVal == 1)
	{
		P = ray(t);
		
		N = (P - sp->getCenter());
		N.normalize();
		texColor = getDiffuse();
	}
	
	return retVal;
}
