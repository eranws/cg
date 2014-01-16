//
//  lights.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : Declerae and implement classes representing ambient and
//            point light sources.
//

#ifndef __LIGHTS__
#define __LIGHTS__


//////////////////////////////
// Project Includes         //
//////////////////////////////

#include "general.h"
#include "MyVecs.h"

#include <vector>
//////////////////////////////
// Class Decleration        //
//////////////////////////////

class AmbientLight 
{
public:
	AmbientLight(Color3d  color = COLOR_BLACK):_color(color){}
public:
	Color3d _color;

};


class PointLight 
{
public:
	PointLight(Point3d position = POINT_ORIGIN,Color3d color = COLOR_BLACK, double radius = EPS):
		_position(position),_color(color), _radius(radius)
	{

		int numRays = ceil(std::max(_radius, 1.0));
		for(int i = 0; i < numRays; i++)
		{
			Vector3d randVec = Vector3d(rand(), rand(), rand()).normalize();
			_shadowRays.push_back(randVec);
		}

		if (_radius > EPS)
		{
			_shadowRays.push_back(Vector3d(0, 0, 1));
			_shadowRays.push_back(Vector3d(0, 0, -1));
			_shadowRays.push_back(Vector3d(0, 1, 0));
			_shadowRays.push_back(Vector3d(0, -1, 0));
			_shadowRays.push_back(Vector3d(1, 0, 0));
			_shadowRays.push_back(Vector3d(-1, 0, 0));
		}
	}

	Point3d _position;
	Color3d _color;
	double _radius;
	std::vector<Vector3d> _shadowRays; 

};


#endif
