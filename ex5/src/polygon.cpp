
/*
 * polygon.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ilansh
 */
#include "polygon.h"

Polygon::Polygon()
{
}

Polygon::~Polygon()
{
}

Polygon::Polygon(vector<Point3d>& vertices) : _vertices(vertices)
{
	triangulate();
}

Polygon::Polygon(vector<Point3d>& vertices, Vector3d& normal)
{
}

Polygon::Polygon(vector<Point3d>& vertices, vector<Point2d> textices)
{
}

Polygon::Polygon(vector<Point3d>& vertices, vector<Point2d> textices,
		Vector3d& normal)
{
}

int Polygon::intersect(Ray& ray, double tMax, double& t, Point3d& P,
		Vector3d& N, Color3d& texColor)
{
	int retVal = 0;

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		Triangle* tri = _triangles[i];
		Plane p = tri->getPlane();

		double c = ray.D() | p.D();
		if (fabs(c) > EPS)
		{
			t = ((p.O() | p.D()) - (p.D() | ray.O())) / (c);
			
			//if (t > tMax) //TODO!!!

			if (t > 0)
			{

			Point2d texCoords; //TODO !!!
			P = ray(t);
			if (tri->isInside(P, texCoords))
			{
				retVal = 1;
				N = p.D();

				if (_diffuseTexture)
				{
					texColor = textureDiffuse(P);
				}
				else
				{
					texColor = getDiffuse();
				}
			}
			}
		}

		if (retVal == 1)
			break; // no need to check more polygons

	}

	return retVal;
}

void Polygon::triangulate()
{
	if (_vertices.size() <= 2)
		return;

	Point3d p0 = _vertices[0];
	for (size_t i = 2; i < _vertices.size(); i++)
	{
		Triangle* t = new Triangle(p0, _vertices[i-1], _vertices[i]);
		_triangles.push_back(t);
	}

}

Color3d Polygon::textureDiffuse(const Point3d& P) const {
	return COLOR_BLACK;
}
