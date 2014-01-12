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
	return (d - (2*(d|n) * n)).normalize();
}

Vector3d refract(Vector3d I, Vector3d N, double mu)
{
	double NI = (N | I);
	double det = 1 - mu * mu * (1 - NI * NI);

	if (det > 0)
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

	if (vis > 0.99)// MINIMAL_VIS )
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
				reflectionColor = obj->getSpecular() * trace_ray(reflected, vis * obj->getReflection().length() * 0.5, obj);  //! TODO vis/2?
			} 			
			
			double muOrigin = 1; 
			if (originObj) // ray started from air or camera
			{
				muOrigin = originObj->getIndex();
			}

			double muOut = 1;
			if (obj != originObj) // the outer material is air
			{
				muOut = obj->getIndex(); //! TODO mu1/mu2
			}
			
			double mu = muOrigin / muOut;
			if (obj->isRefractive())
			{
				Ray refracted(P, refract(ray.D(), N, mu));
				refractionColor = trace_ray(refracted, vis *  obj->getTransparency().length() * 0.5, obj);
			}
						
			retColor = obj->getAmbient() * _ambientLight._color;

			//return retColor;

			for (int i=0; i < _lights.size(); i++)
			{
				Vector3d L = (_lights[i]->_position - P).normalize();

				bool isShadow = findNearestObject(Ray(P,L));
				
				if (isShadow) 
					0;
					//continue;


				if ((L | N) > 0)
				{
					Color3d diffuse = obj->getDiffuse() * _lights[i]->_color * (L | N);					
					retColor += diffuse;
				
				}
					Vector3d R = reflect(L, N);
					Color3d specular = obj->getSpecular() * _lights[i]->_color * pow(std::max(ray.D() | R, EPS), obj->getShining());
								
					retColor += specular;
								
			}

			//retColor *= (COLOR_WHITE - obj->getTransparency()) * (COLOR_WHITE - obj->getReflection());

			/*
			retColor +=	reflectionColor * obj->getReflection() * (COLOR_WHITE - obj->getTransparency()) + 
			refractionColor * obj->getTransparency();
			*/

//			retColor = 
//				texColor * (COLOR_WHITE - obj->getTransparency()) + 
//				reflectionColor +
//				refractionColor ;

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
