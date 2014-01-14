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
	Vector3d spherePosition(0, 0, sphereRadius - _w);

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
	if (t21 > t11)
	{
		if (t21 > EPS)
		{
			if(_sphere1.isInside(ray(t21)))
				retVal = 1;
			t = t21;
			sp = &_sphere2;
		}
	}
	else
	{
		if (t11 > EPS)
		{
			if(_sphere2.isInside(ray(t11)))
				retVal = 1;
			t = t11;
			sp = &_sphere1;
		}
	}

	if (!retVal)
	{

		if (t12 > t22)
		{
			if (t12 > EPS)
			{
				if(_sphere2.isInside(ray(t12)))
					retVal = 1;
				t = t12;
				sp = &_sphere1;
			}
		}
		else
		{
			if (t22 > EPS)
			{
				if(_sphere1.isInside(ray(t22)))
					retVal = 1;
				t = t22;
				sp = &_sphere2;
			}
		}



	}


	/*
	if(abs(t1) < EPS || abs(t2) < EPS || abs(t3) < EPS || abs(t4) < EPS) {

	}

	if ray from out of lens
	find the third largest t and its sphere
	else
	find the second largest t and its sphere
	*/

	if (retVal == 1)
	{
		P = ray(t);

		N = (P - sp->getCenter());
		N.normalize();
		texColor = getDiffuse();
	}

	return retVal;
}
