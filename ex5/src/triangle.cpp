/*
 * triangle.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ilansh
 */


#include "triangle.h"

Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2) {
}

Triangle::Triangle(Point3d p0, Point3d p1, Point3d p2, Point2d t0, Point2d t1,
		Point2d t2) {
}

Triangle::~Triangle() {
}

bool Triangle::isInside(const Point3d& p, Point2d& texCoord) const {
	return false; //TODO
}




