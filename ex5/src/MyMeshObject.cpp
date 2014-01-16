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
							Vector3d& N, Color3d& texColor) const
{
	bool intersectBoundingSphere = _boundingSphere->getRoots(ray);
	if (!intersectBoundingSphere)
	{
		return 0;
	}

	int retVal = 0;
	for (size_t i = 0; i < _polygons.size(); i++)
	{
		int intersectPolygon = _polygons[i].intersect(ray, tMax, t, P, N, texColor);
		if (intersectPolygon == 1)
		{
			tMax = t;
			retVal = 1;
		}
	}

	return retVal;




}


void MyMeshObject::populatePolygons()
{
	_mesh.request_face_normals();
	_mesh.update_normals();

	_mesh.request_vertex_texcoords2D();
	bool textured = _mesh.has_vertex_texcoords2D();

	bool hasNormal =_mesh.has_face_normals();

	for (MyMesh::FaceIter h_it=_mesh.faces_begin(); h_it!=_mesh.faces_end(); ++h_it)
	{
		vector<Point3d> vertices;
		vector<Point2d> textices;
		// circulate around the current face
		for (MyMesh::FaceVertexIter fv_it = _mesh.fv_iter(h_it); fv_it; ++fv_it)
		{
			MyMesh::Point p = _mesh.point(fv_it.handle());
			vertices.push_back(p);

			if (textured)
			{
				MyMesh::TexCoord2D uv = _mesh.texcoord2D(fv_it);// vhandle is a VertexHandle
				textices.push_back(uv);
			}
		}

		if (textured)
		{
			if (hasNormal)
			{
				Vector3d n = _mesh.normal(h_it);
				_polygons.push_back(Polygon(vertices, textices, n));
			}
			else
			{
				_polygons.push_back(Polygon(vertices, textices));
			}
		}
		{
			if (hasNormal)
			{
				Vector3d n = _mesh.normal(h_it);
				_polygons.push_back(Polygon(vertices, n));
			}
			else
			{
				_polygons.push_back(Polygon(vertices));
			}
		}
	}
	_mesh.release_face_normals();
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
		radius = std::max(radius, p.length());
	}
	_boundingSphere = new Sphere(center, radius + EPS);

}

void MyMeshObject::set_texture_map(BImage* image)
{
	_diffuseTexture = image;
	for (int i=0; i<_polygons.size(); i++)
	{
		_polygons[i].set_texture_map(image);
	}
}