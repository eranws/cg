//
//  sphere.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class that represents a sphere on the 3d space.
//            Inherits from Object class, implementing the method to test
//            intersection of a given ray with the sphere.
//

#ifndef _LENS_HH_
#define _LENS_HH_


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "object.h"

#include "sphere.h"

//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Lens : public Object 
{
public:
  // Constructor - create a sphere with the given parameters  //
	Lens(Point3d center = POINT_ORIGIN, double radius = 1.0, double width = 1.0);

	virtual ~Lens();


	virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) const;

	Point3d _center;
	double _r; //radius
	double _w; //width

	Sphere _sphere1;
	Sphere _sphere2;
};

#endif /* _SPHERE_HH */
