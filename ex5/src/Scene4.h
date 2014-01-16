//
//  Scene4.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//

#ifndef cg_projects_Scene4_h
#define cg_projects_Scene4_h

#include "Lens.h"


struct Scene4 : public Scene
{
	Scene4()
	: Scene()
	{
		defineGeometry();
		defineLights();
	}
	
	void defineLights()
	{
		Scene & scene = *this;
		Point3d pos(10,100,10);
		Color3d color(1,1,1);
		PointLight  * p = new PointLight(pos,color, 1);
		scene.add_light(p);
		
		Point3d pos1(10,20,-10);
		Color3d color1(1,1,1);
		PointLight  * p1 = new PointLight(pos1,color1);
		scene.add_light(p1);
	}
	
	void defineGeometry()
	{
		Scene & scene = *this;
#if !WITHOUT_TEXTURES
		BImage * b = new BImage("textures/checkerboard_lg.bmp");
		BImage * w = new BImage("textures/warning.bmp");
#endif
		
		/* define some colors */
		Color3d white(1.0);
		Color3d black(0.0);
		Color3d red(1,0,0.0);
		Color3d green(0,1.0,0.0);
		Color3d blue(0,0,1.0);
		
		
		Point3d center(0.37,0.5,1);
		double radius = 2;
		Ellipsoid * sp = new Ellipsoid(center,radius, Vector3d(1.0, 0.5, 0.25));
		sp->diffuse() = white;
		sp->reflection() = black;
		sp->specular() = white;
		sp->shining() = 16;
#if !WITHOUT_TEXTURES
		sp->set_texture_map(b);
#endif
		scene.add_object(sp);

		scene.backgroundColor() = (blue +  white) * 0.5;


		//Make a pair of glasses!!

		//lens1
		Point3d centerLens1(0, 0.5, 0);
		double radiusLens1 = 0.25;
		Lens * lens1 = new Lens(centerLens1, radiusLens1, radiusLens1 / 4);
		lens1->diffuse() = blue;
		lens1->reflection() = white * 0.2;
		lens1->transparency() = white * 0.5;
		lens1->index() = 1.5;
		lens1->specular() = white;
		lens1->shining() = 16;
		scene.add_object(lens1);

		//lens2
		Point3d centerLens2(0.75, 0.5, 0);
		double radiusLens2 = 0.25;
		Lens * lens2 = new Lens(centerLens2, radiusLens2, radiusLens1 / 4);
		lens2->diffuse() = blue;
		lens2->reflection() = white * 0.2;
		lens2->transparency() = white * 0.5;
		lens2->index() = 1.5;
		lens2->specular() = white;
		lens2->shining() = 16;
		scene.add_object(lens2);


		//Connecting plane
		std::vector<Point3d> plane1(4);
		plane1[0] = Point3d(0.25,0.5,0);
		plane1[1] = Point3d(0.25,0.55,0);
		plane1[2] = Point3d(0.52,0.55,0);
		plane1[3] = Point3d(0.52,0.5,0);
		Polygon * poly1 = new Polygon(plane1);
		poly1->diffuse() = ((green + red) * 0.8 + white * 0.5) * 0.2;
		poly1->reflection() = (green + red) * 0.8 + white * 0.5;
		scene.add_object(poly1);

		//arm 1
		std::vector<Point3d> plane2(4);
		plane2[0] = Point3d(-0.25,0.5,0);
		plane2[1] = Point3d(-0.25,0.55,0);
		plane2[2] = Point3d(-0.25,0.55,2);
		plane2[3] = Point3d(-0.25,0.5,2);
		Polygon * poly2 = new Polygon(plane2);
		poly2->diffuse() = ((green + red) * 0.8 + white * 0.5) * 0.2;
		poly2->reflection() = (green + red) * 0.8 + white * 0.5;
		scene.add_object(poly2);

		//arm 2
		std::vector<Point3d> plane3(4);
		plane3[0] = Point3d(1,0.5,0);
		plane3[1] = Point3d(1,0.55,0);
		plane3[2] = Point3d(1.5,0.55,2);
		plane3[3] = Point3d(1.5,0.5,2);
		Polygon * poly3 = new Polygon(plane3);
		poly3->diffuse() = ((green + red) * 0.8 + white * 0.5) * 0.2;
		poly3->reflection() = (green + red) * 0.8 + white * 0.5;
		scene.add_object(poly3);





		MyMesh mesh;
		mesh.request_vertex_texcoords2D();
		if ( ! OpenMesh::IO::read_mesh(mesh, "meshes/girl_face.obj"))
		{
			fprintf(stderr, "Error: cannot read mesh from file\n");
			exit(-1);
		}
		MyMeshObject * mo = new MyMeshObject(mesh);
		mo->diffuse() = white;
		mo->reflection() = black;
		//  mo->transparency() = white * 0.5;
		mo->specular() = black;
		mo->shining() = 0;
		mo->index() = 1.0;
#if !WITHOUT_TEXTURES
		mo->set_texture_map(w);
#endif
//		scene.add_object(mo);
		
		//create a plane
		std::vector<Point3d> plane(4);
		double x = 100;
		double z = -4;
		plane[0] = Point3d(-x,z,-x);
		plane[1] = Point3d(-x,z,x);
		plane[2] = Point3d(x,z,x);
		plane[3] = Point3d(x,z,-x);
		Polygon * poly = new Polygon(plane);
		poly->diffuse() = ((blue + red) * 0.5 + white * 0.5) * 0.2;
		poly->reflection() = (blue + red) * 0.5 + white * 0.5;
		scene.add_object(poly);
	}
	
	virtual void setDefaultCamera(Camera& camera) const
	{
		Point3d pos(1,3,-4);
		double fov_h = 20 / 180.0 * M_PI;
		Point3d coi(0,0,-0);
		Vector3d up(0,1,0) ;
		camera = Camera(pos,coi,up,fov_h);
	}
	
	virtual ~Scene4() {

	}

};

#endif
