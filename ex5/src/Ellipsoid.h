//
//  Ellipsoid.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : A class that represents an Ellipsoid on the 3d space.
//            Inherits from Object class, implementing the method to test
//            intersection of a given ray with the Ellipsoid.
//

#ifndef _ELLIPSOID_HH
#define _ELLIPSOID_HH


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "object.h"


//////////////////////////////
// Class Decleration        //
//////////////////////////////

class Ellipsoid : public Object 
{
public:

	// Constructor - create a Ellipsoid with the given parameters  //
	Ellipsoid(Point3d C = POINT_ORIGIN, double r = 1, Vector3d _scale = Vector3d(1,1,1));

	// Destructor - delete a Ellipsoid  //
	virtual ~Ellipsoid();

	// Ray Ellipsoid intersection //
	virtual int intersect(IN Ray& ray, IN double tMax, OUT double& t, OUT Point3d& P, OUT Vector3d& N, OUT Color3d& texColor) const;

	Color3d textureDiffuse(const Point3d& P) const;

	bool Ellipsoid::getRoots(const Ray& ray, double* t0, double* t1) const;

	Vector3d getCenter() const {return _C;}

protected:

	Point3d   _C; // The Ellipsoid's center  //
	double    _r; // The Ellipsoid's underlying sphere radius  //

	//scale factor for x,y,z axis, respectively
	Vector3d _scale;
	Vector3d _scaleInv; //optimization


};

#endif /* _ELLIPSOID_HH */
