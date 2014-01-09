#include "MyMeshObject.h"

MyMeshObject::MyMeshObject(MyMesh& mesh)
{
	_mesh = mesh;
	populatePolygons();
	calculateBoundingSphere();
}

MyMeshObject::~MyMeshObject()
{
	if (_boundingSphere)
	{
		delete(_boundingSphere);
	}
}

int MyMeshObject::intersect(Ray& ray, double tMax, double& t, Point3d& P,
		Vector3d& N, Color3d& texColor)
{
	int intersectBoundingSphere = _boundingSphere->intersect(ray, tMax, t, P, N, texColor);
	if (intersectBoundingSphere == 0)
	{
		//return 0;
	}

	int retVal = 0;
	double closestT = INF;
	for (size_t i = 0; i < _polygons.size(); i++)
	{
		Color3d texColor2;
		int intersectPolygon = _polygons[i].intersect(ray, tMax, t, P, N, texColor2);
	
		if (intersectPolygon == 1)
		{
		//TODO: optimize OOP stuff with scene
		if (t < closestT)
		{
			texColor = texColor2;
			closestT = t;
			
			retVal = 1;
		}
		}
	}

	return retVal;




}


void MyMeshObject::populatePolygons()
{
		//_mesh.request_face_normals();
        //_mesh.update_normals();


        for (MyMesh::FaceIter h_it=_mesh.faces_begin(); h_it!=_mesh.faces_end(); ++h_it)
        {
			vector<Point3d> vertices;
                // circulate around the current face
                for (MyMesh::FaceVertexIter fv_it = _mesh.fv_iter(h_it); fv_it; ++fv_it)
                {
                        MyMesh::Point p = _mesh.point(fv_it.handle());
						vertices.push_back(p);

                        //MyMesh::Normal n = _mesh.normal(h_it);
                        //glm::vec4 normal(n[0], n[1], n[2], 1.0);
                        

                }
				_polygons.push_back(Polygon(vertices));
        }
        //_mesh.release_face_normals();
}

void MyMeshObject::calculateBoundingSphere()
{
	Point3d center = Point3d(0, 0, 0);
	double radius = 0.0;

    MyMesh::VertexIter vertexIter;
	
	int vNum = _mesh.n_vertices();
    for (vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter)
	{
		center += _mesh.point(vertexIter);
	}
    center /= (double)vNum;

	for (vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter)
	{
		Point3d p = center - _mesh.point(vertexIter);
		if (p.sqrnorm() > radius) radius = p.sqrnorm();
	}
	radius = sqrt(radius);

	_boundingSphere = new Sphere(center, radius);

}
