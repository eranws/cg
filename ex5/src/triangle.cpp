/*
 * triangle.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ilansh
 */


#include "triangle.h"

Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2) : _p0(p0), _u(p1-p0), _v(p2-p0)
{
	_n = (_u % _v).normalize();
}

Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2, Point2d t0, Point2d t1,	Point2d t2)
{
	//TODO
}

Triangle::~Triangle() {
}

bool Triangle::isInside(const Point3d& p, Point2d& texCoord) const
{
	return	(((_u % (p - _p0)) | _n) >= 0	&&
			(((p - _p0) % _v )| _n) >= 0	&&
			(((_v -_u) % (p - _p0 - _u)) | _n) >= 0);
}

Plane Triangle::getPlane()
{
	return Plane(_p0, _n);
}




