
/*
 * polygon.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: ilansh
 */
#include "polygon.h"

Polygon::Polygon() {
}

Polygon::~Polygon() {
}

Polygon::Polygon(vector<Point3d>& vertices) {
}

Polygon::Polygon(vector<Point3d>& vertices, Vector3d& normal) {
}

Polygon::Polygon(vector<Point3d>& vertices, vector<Point2d> textices) {
}

Polygon::Polygon(vector<Point3d>& vertices, vector<Point2d> textices,
		Vector3d& normal) {
}

int Polygon::intersect(Ray& ray, double tMax, double& t, Point3d& P,
		Vector3d& N, Color3d& texColor)
{
	return 0; //TODO;
}

void Polygon::triangulate() {
}



