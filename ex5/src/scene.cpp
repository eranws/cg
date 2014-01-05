/*
* scene.cpp
*
*  Created on: Jan 2, 2014
*      Author: ilansh
*/

#include "scene.h"

Scene::Scene(){

}

Scene::Scene(Color3d& color, AmbientLight& light, double cutoffAngle) :
	_background(color),
	_ambientLight(light),
	_cutoffAngle(cutoffAngle)
{
}

Color3d Scene::trace_ray(Ray ray, double vis) const
{
	Color3d retColor;

	if (vis < MINIMAL_VIS)
	{
		retColor = COLOR_BLACK;
	}
	else
	{
		Object* obj;
		double t;
		Point3d P;
		Vector3d N;
		Color3d texColor;

		bool isFound = findNearestObject(ray, &obj, t, P, N, texColor);

		if (isFound)
		{
			Color3d ambient = obj->getAmbient();
			retColor = ambient;
		}
		else
		{
			retColor = _background;
		}
	}

	return retColor;
}
void Scene::add_object(Object* obj)
{
	_objects.push_back(obj);
}

void Scene::add_light(PointLight* light)
{
	_lights.push_back(light);
}

Ray Scene::perturbateRay(const Ray& r) const
{
	return Ray(); //TODO
}

bool Scene::findNearestObject(Ray ray, Object** object, double& t, Point3d& P,
							  Vector3d& N, Color3d& texColor) const
{
	bool retVal = false;
	object = NULL;
	for (int i=0; i < _objects.size(); i++)
	{
		Object* it = _objects[i];
		int isIntersect = it->intersect(ray, INF, t, P, N, texColor);
		if (isIntersect == 1)
		{
			//TODO: check if closer than current
			retVal = true;
		}

	}

	return retVal;
}

Color3d Scene::calcReflection(const Ray& ray, const Point3d& P,
							  const Vector3d& N, const Object& object, double vis,
							  bool isCritical) const
{
	return COLOR_BLACK; //TODO
}

Color3d Scene::calcRefraction(const Ray& ray, const Point3d& P,
							  const Vector3d& N, const Object& object, double vis) const
{
	return COLOR_BLACK; //TODO
}
