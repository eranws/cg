//
//  triangle.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class that represents a triangle on the 3d space.
//            Implements a method checking if a given point is inside
//            the triangle.
//

#ifndef _TRIANGLE_HH
#define _TRIANGLE_HH


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "MyVecs.h"
#include "ray.h"

//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Triangle 
{
public:
	// Constructor - create a triangle from the given points  //
	Triangle(Point3d p0, Point3d p1, Point3d p2);
	Triangle(Point3d p0, Point3d p1, Point3d p2, Point2d t0, Point2d t1, Point2d t2);

	// isInside - return true if the given point is inside the triangle //
	bool isInside(IN const Point3d& p, OUT Point2d& texCoord) const;

	Plane getPlane();

	void setNormal(Vector3d n){_n = n;}

	Point2d t0(){return _t0;}  // The texture map coordiate of p0  //
	Point2d tu(){return _tu;}  // The texture map coordiate of p1  //
	Point2d tv(){return _tv;}  // The texture map coordiate of p2  //

private:
	void init();

	Point3d _p0;  // One of the triangle vertices   //
	Point3d _p1;  // One of the triangle vertices   //
	Point3d _p2;  // One of the triangle vertices   //

	Vector3d _n;  // the triangle normal Plane //
	double _area;

	Point2d _t0;  // The texture map coordiate of p0  //
	Point2d _tu;  // The texture map coordinate of p1 //
	Point2d _tv;  // The texture map coordinate of p2 //

};

#endif /* _TRIANGLE_HH */
