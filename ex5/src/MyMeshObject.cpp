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
	for (int i = 0; i < _polygons.size(); i++)
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

void MyMeshObject::calculateBoundingSphere()
{
	Point3d center;
	double radius = 0.0;

	_boundingSphere = new Sphere(center, radius);
}


void MyMeshObject::populatePolygons()
{
		//_mesh.request_face_normals();
        //_mesh.update_normals();


        size_t i = 0;
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