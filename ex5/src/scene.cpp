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
				reflectionColor = obj->getSpecular() * trace_ray(reflected, vis * RECURSION_FACTOR, obj);  //! TODO vis/2?
			} 			

			
			if (obj->isRefractive() || (originObj && originObj->isRefractive()))
			{
				double muOrigin = 1;
				Vector3d refractiveNormal;
				if (originObj == obj) // inside object, the outer material is air
				{
					muOrigin = originObj->getIndex();
					refractiveNormal = -N;
				}

				double muOut = 1;
				if (obj != originObj) // 
				{
					muOut = obj->getIndex(); //! TODO mu1/mu2
					refractiveNormal = N;
				}

				double mu = muOrigin / muOut;

				Ray refracted(P, refract(ray.D(), refractiveNormal, mu));

				//std::cout << (refracted.D() | ray.D()) << std::endl;
				//Ray refracted(P, ray.D());
				
				refractionColor = trace_ray(refracted, vis * RECURSION_FACTOR, obj);
			}

			

			Color3d sumLights = obj->getAmbient() * _ambientLight._color;

			for (size_t i = 0; i < _lights.size(); i++)
			{
				Vector3d L = (_lights[i]->_position - P).normalize();

				bool isShadow = findNearestObject(Ray(P,L));

				if (isShadow) 
					continue;


				if ((L | N) > EPS)
				{
					Color3d diffuse = obj->getDiffuse() * _lights[i]->_color * (L | N);					
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
	return Ray(); //TODO
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

	int dbgIntersectCount = 0;

	for (size_t i=0; i < _objects.size(); i++)
	{
		const Object* it = _objects[i];
		Color3d texColor2;
		int isIntersect = it->intersect(ray, INF, t, P, N, texColor2);
		if (isIntersect == 1)
		{
			dbgIntersectCount++;
			if (dbgIntersectCount > 1)
			{
				dbgIntersectCount++;
			}

			if (t < closestT)
			{
				texColor = texColor2;
				closestT = t;
				*object =  _objects[i];
				retVal = true;
			}
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
