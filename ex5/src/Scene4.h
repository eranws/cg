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
		Point3d pos(0,5,0);
		Color3d color(1,1,1);
		PointLight  * p = new PointLight(pos,color, 1.5);
		scene.add_light(p);

		Point3d pos2(1,1,-3);
		PointLight  * p2 = new PointLight(pos,color);
		scene.add_light(p2);

		Point3d pos1(-1,13,0);
		Color3d color1(1,1,1);
		PointLight  * p1 = new PointLight(pos1,color1);
		scene.add_light(p1);
	}
	
	void defineGeometry()
	{
		Scene & scene = *this;
#if !WITHOUT_TEXTURES

		BImage * warnTex = new BImage("textures/warning.bmp");
		BImage * woodTex = new BImage("textures/wood.bmp");
#endif
		
		/* define some colors */
		Color3d white(1.0);
		Color3d black(0.0);
		Color3d red(1,0,0.0);
		Color3d green(0,1.0,0.0);
		Color3d blue(0,0,1.0);
		
		
		//Ellipsoid
		Point3d center(-1,2,1);
		double radius = 0.5;
		Ellipsoid * sp = new Ellipsoid(center,radius, Vector3d(4, 1, 1));
		sp->diffuse() = white;
		//sp->reflection() = black;
		//sp->specular() = white;
		//sp->shining() = 2;
#if !WITHOUT_TEXTURES
		sp->set_texture_map(warnTex);
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
		plane2[2] = Point3d(-0.25,0.3,2);
		plane2[3] = Point3d(-0.25,0.25,2);
		Polygon * poly2 = new Polygon(plane2);
		poly2->diffuse() = ((green + red) + white * 0.8) * 0.2;
		poly2->reflection() = (green + red) + white * 0.5;
		scene.add_object(poly2);

		//arm 2
		std::vector<Point3d> plane3(4);
		plane3[0] = Point3d(1,0.5,0);
		plane3[1] = Point3d(1,0.55,0);
		plane3[2] = Point3d(1.5,0.3,2);
		plane3[3] = Point3d(1.5,0.25,2);
		Polygon * poly3 = new Polygon(plane3);
		poly3->diffuse() = ((green + red) + white * 0.8) * 0.2;
		poly3->reflection() = (green + red) + white * 0.5;
		scene.add_object(poly3);
	
		//create a plane
		std::vector<Point3d> plane(4);
		double x = -5;
		double y = 0.22;
		double z = -4;
		plane[0] = Point3d(-x,y,-z);
		plane[1] = Point3d(-x,y,z);
		plane[2] = Point3d(x,y,z);
		plane[3] = Point3d(x,y,-z);
			
		vector<Point2d> plane_uv(4);
		plane_uv[0] = Point2d(0,1);
		plane_uv[1] = Point2d(1,1);
		plane_uv[2] = Point2d(1,0);
		plane_uv[3] = Point2d(0,0);

		Polygon * poly = new Polygon(plane, plane_uv);
		//poly->diffuse() = red * 0.5 + white * 0.5;
//		poly->reflection() = (blue + red) * 0.5 + white * 0.5;
		poly->ambient() = red;
		//poly->specular()  = white;
		poly->diffuse() = white/2;
		poly->reflection() = white/8;
		poly->specular() = white/4;
		poly->shining() = 2;

		poly->set_texture_map(woodTex);

		scene.add_object(poly);
	}
	
	virtual void setDefaultCamera(Camera& camera) const
	{
		Point3d pos(1,2,-4);
		double fov_h = 30 / 180.0 * M_PI;
		Point3d coi(0,0, 0);
		Vector3d up(0,1,0) ;
		camera = Camera(pos,coi,up,fov_h);
	}
	
	virtual ~Scene4() {

	}

};

#endif
