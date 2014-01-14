/*
* triangle.cpp
*
*  Created on: Jan 2, 2014
*      Author: ilansh
*/


#include "triangle.h"

void Triangle::init()
{
	Vector3d _u = (_p1 - _p0);  // a vector from p0 to p1         //
	Vector3d _v = (_p2 - _p0);  // a vector from p0 to p2         //

	_n = (_u % _v).normalize();
	_area = (_u % _v).norm();
}

Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2) : _p0(p0), _p1(p1) ,_p2(p2)
{
	_textured = false;
	init();
}

Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2, Point2d t0, Point2d t1,	Point2d t2) : _p0(p0), _p1(p1) ,_p2(p2), _t0(t0), _tu(t1) ,_tv(t2)
{
	_textured = true;
	init();
}


bool Triangle::isInside(const Point3d& P, Point2d& texCoord) const
{
	Vector3d e1 = _p0 - P, e2 = _p1 - P, e3 = _p2 - P;

	texCoord[X] = (e1 % e3).norm() / _area;
	texCoord[Y] = (e1 % e2).norm() / _area;
	double w = (e2 % e3).norm() / _area;

	return (texCoord[X] + texCoord[Y] + w - 1 < EPS);
}

Plane Triangle::getPlane()
{
	return Plane(_p0, _n);
}