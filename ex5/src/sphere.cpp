/*
 * sphere.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ilansh
 */

#include "sphere.h"


Sphere::Sphere() : Object(), _C(Point3d()), _r(1.0)
{
}

Sphere::Sphere(Point3d C, double r)  : _C(C), _r(r)
{
}

Sphere::~Sphere()
{
}


int Sphere::intersect(Ray& ray, double tMax, double& t, Point3d& P, Vector3d& N,
		Color3d& texColor)
{
	int retVal = 0;

	//Compute A, B and C coefficients
	using OpenMesh::dot;
    double a = dot(ray.D(), ray.D()); //should be 1
	double b = 2 * dot(ray.D(), ray.O() - _C);
    double c = dot(ray.O() - _C, ray.O() - _C) - (_r * _r);

    //Find discriminant
    double disc = b * b - 4 * a * c;
    
    // if discriminant is negative there are no real roots, so return 
    // false as ray misses sphere
    if (disc < 0)
        return 0;

    // compute q as described above
    double distSqrt = sqrt(disc);
    double t0 = (-b - distSqrt) * 0.5;
    double t1 = (-b + distSqrt) * 0.5;

	//TODO: check for problems t should be smaller of the two.

	if (t0 > t1)
    {
        std::swap(t0, t1); // if t0 is bigger than t1 swap them around
    }

    if (t0 >= 0)
	{
		retVal = 1;
		t = t0;
	}
    else if (t1 >= 0)
    {
		retVal = 1;
        t = t1;
    }


	if (retVal == 1)
	{
		P = ray(t);
		N = P - _C;

		if (_diffuseTexture)
		{
			texColor = textureDiffuse(P);
		}
		else
		{
			texColor = getDiffuse();
		}

	}
	
	return retVal;
}

Color3d Sphere::textureDiffuse(const Point3d& P) const {
	return COLOR_BLACK; //TODO (phi theta)
}
