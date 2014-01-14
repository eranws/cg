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
	_ambientLight(light),
	_background(color),
	_cutoffAngle(cutoffAngle)
{
}

Vector3d reflect(Vector3d d, Vector3d n)
{
	d.normalize();
	n.normalize();

	return (d - (2*(d|n) * n)).normalize();
}

Vector3d refract(Vector3d I, Vector3d N, double mu)
{

	I = -I;

	I.normalize();
	N.normalize();

	double NI = (N | I);
	double det = 1 - mu * mu * (1 - NI * NI);

	if (det > EPS)
	{
		return N * (mu * NI - sqrt(det)) - mu * I;
	}
	else
	{
		return reflect(I, N);
	}
}



Color3d Scene::trace_ray(Ray ray, double vis, const Object* originObj) const
{
	Color3d retColor = _background;
	Color3d reflectionColor = COLOR_BLACK;
	Color3d refractionColor = COLOR_BLACK;

	ray = Ray(ray(EPS*10), ray.D());

#define dbg(x) {std::cout << #x << ":" << x << " ";}

	if (vis > MINIMAL_VIS )
	{
		const Object* obj = 0;
		double t;
		Point3d P;
		Vector3d N;
		Color3d texColor;

		bool isFound = findNearestObject(ray, &obj, t, P, N, texColor);
		if (isFound)
		{

			if (obj->getReflection() != COLOR_BLACK)
			{
				Ray reflected(P, reflect(ray.D(), N));

				for (int s = 0; s < _numberOfRefRays; s++)
				{
					reflectionColor += obj->getSpecular() * trace_ray(reflected, vis * RECURSION_FACTOR, obj);
				}
				reflectionColor /= _numberOfRefRays;
			} 			


			if (obj->isRefractive() || (originObj && originObj->isRefractive()))
			{
				double mu = 1.0 / obj->getIndex();

				Vector3d refractiveNormal = N;
				if (originObj == obj) // inside object
				{
					mu = 1.0 / mu;
					refractiveNormal = -N;				
				}

				Ray refracted(P, refract(ray.D(), refractiveNormal, mu));

				for (int s = 0; s < _numberOfRefRays; s++)
				{
					refractionColor += trace_ray(refracted, vis * RECURSION_FACTOR, obj);
				}
				refractionColor /= _numberOfRefRays;				
			}



			Color3d sumLights = obj->getAmbient() * _ambientLight._color;

			for (size_t i = 0; i < _lights.size(); i++)
			{

				
				bool isShadow = true;

				Vector3d L;

				for (size_t s = 0; s < _lights[i]->_shadowRays.size(); s++)
				{
					L = (_lights[i]->_position + _lights[i]->_shadowRays[s] * _lights[i]->_radius - P).normalize();
					isShadow = findNearestObject(Ray(P,L));

					if (!isShadow)
					{
						break;
					}
				}


				if (isShadow)
				{
					continue;
				}


				if ((L | N) > EPS)
				{
					Color3d diffuse = texColor * _lights[i]->_color * (L | N);					
					sumLights += diffuse;
				}
				Vector3d R = reflect(L, N);
				Color3d specular = obj->getSpecular() * _lights[i]->_color * pow(std::max(ray.D() | R, EPS), obj->getShining());

				sumLights += specular;

			}

			retColor = sumLights / (_lights.size() + 1); //avoid overflow, TODO weight light by distance, +1 for ambient
			retColor *= (COLOR_WHITE - obj->getTransparency()) * (COLOR_WHITE - obj->getReflection());

			retColor += reflectionColor * obj->getReflection() * (COLOR_WHITE - obj->getTransparency());
			retColor += refractionColor * obj->getTransparency();
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
	Vector3d pert(rand(), rand(), rand());
	pert.normalize();
	pert *= sin(M_PI * _cutoffAngle / 180);

	return Ray(r.O(), r.D() + pert);
}


bool Scene::findNearestObject(Ray ray) const
{
	const Object* obj = 0;
	double t;
	Point3d P;
	Vector3d N;
	Color3d texColor;

	return findNearestObject(ray, &obj, t, P, N, texColor);
}


bool Scene::findNearestObject(Ray ray, const Object** object, double& t, Point3d& P,
							  Vector3d& N, Color3d& texColor) const
{
	bool retVal = false;
	double closestT = INF;


	for (size_t i=0; i < _objects.size(); i++)
	{
		const Object* it = _objects[i];
		Color3d texColor2;
		int isIntersect = it->intersect(ray, closestT, t, P, N, texColor2);
		if (isIntersect == 1 && t < closestT)
		{
				texColor = texColor2;
				closestT = t;
				*object =  _objects[i];
				retVal = true;
		}

	}

	return retVal;
}