
/*
* polygon.cpp
*
*  Created on: Jan 2, 2014
*      Author: ilansh
*/
#include "polygon.h"

Polygon::Polygon(vector<Point3d>& vertices) : Object(), _vertices(vertices), _externalNormal(false)
{
	triangulate();
}

Polygon::Polygon(vector<Point3d>& vertices, Vector3d& normal) : Object(), _vertices(vertices), _normal(normal), _externalNormal(true)
{
	triangulate();
}

Polygon::Polygon(vector<Point3d>& vertices, vector<Point2d> textices) : Object(), _textices(textices), _vertices(vertices), _externalNormal(false)
{
	
	triangulate();
}

Polygon::Polygon(vector<Point3d>& vertices, vector<Point2d> textices,
				 Vector3d& normal) : Object(), _textices(textices), _vertices(vertices),  _normal(normal), _externalNormal(true)
{
	triangulate();
}

int Polygon::intersect(Ray& ray, double tMax, double& t, Point3d& P,
					   Vector3d& N, Color3d& texColor) const
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

			if (t > 0 && t < tMax)
			{

				Point2d texCoords;
				P = ray(t);
				if (tri->isInside(P, texCoords))
				{
					retVal = 1;
					N = p.D();

					texColor = getDiffuse();
					if (_diffuseTexture)
					{
						texColor *= textureDiffuse(P, texCoords, i);
					}
				}
			}
		}

		if (retVal == 1)
			break; // no need to check more triangles in polygon

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
		Triangle* t;
		if (!_textices.empty())
		{
			 t = new Triangle(p0, _vertices[i-1], _vertices[i], _textices[0], _textices[i-1], _textices[i]);
		}
		else
		{
			t = new Triangle(p0, _vertices[i-1], _vertices[i]);
		}

		if (_externalNormal)
		{
			t->setNormal(-_normal);
		}

		_triangles.push_back(t);
	}

}

Color3d Polygon::textureDiffuse(const Point3d& P, const Point2d& tex, int triIndex) const
{

	double u1 = tex[0], v1 = tex[1];

	double u,v;
	if (_textices.empty()) // Does the polygon have a texture map coordinates  //
	{
		u = u1;
		v = v1;
	}
	else
	{
		u = _triangles[triIndex]->t0()[X] * (1 - u1 - v1) + _triangles[triIndex]->tu()[X] *u1 + _triangles[triIndex]->tv()[X] * v1;
		v = _triangles[triIndex]->t0()[Y] * (1 - u1 - v1) + _triangles[triIndex]->tu()[Y] *u1 + _triangles[triIndex]->tv()[Y] * v1;
	}

	BImage* texture = const_cast<BImage*>(getDiffuseTexture()); //HACK, BImage doesn't support const operations...
	
	u *= texture->width();
	v *= texture->height();

	Bpixel pix = (*texture)(u, v);
	return Color3d(pix.r, pix.g, pix.b) * COLOR_NORMALIZE;
}
