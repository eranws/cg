/*
 * scene.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ilansh
 */

#include "scene.h"

Scene::Scene(){

}

Scene::Scene(Color3d& color, AmbientLight& light, double cutoffAngle) {
}

Color3d Scene::trace_ray(Ray ray, double vis) const
{
	return COLOR_BLACK; //TODO
}

void Scene::add_object(Object* obj)
{
}

void Scene::add_light(PointLight* light)
{
}

Ray Scene::perturbateRay(const Ray& r) const
{
	return Ray(); //TODO
}

bool Scene::findNearestObject(Ray ray, Object** object, double& t, Point3d& P,
		Vector3d& N, Color3d& texColor) const
{
	return false; //TODO
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
