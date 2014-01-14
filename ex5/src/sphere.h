//
//  sphere.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class that represents a sphere on the 3d space.
//            Inherits from Object class, implementing the method to test
//            intersection of a given ray with the sphere.
//

#ifndef _SPHERE_HH
#define _SPHERE_HH


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "object.h"

#include "Ellipsoid.h"

//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Sphere : public Ellipsoid 
{
public:
  // Constructor - create a sphere with the given parameters  //
	Sphere(Point3d C = POINT_ORIGIN, double r = 1.0) : Ellipsoid(C, r) //sphere is an ellipsoid with equal scale factor in all axes (1)
	{	
	};

	bool isInside(Point3d p) const
	{
		return (p-_C).length() < _r;
	}

};

#endif /* _SPHERE_HH */
