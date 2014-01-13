//
//  Scene4.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//

#ifndef cg_projects_Scene4_h
#define cg_projects_Scene4_h

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
		PointLight  * p = new PointLight(pos,color);
		scene.add_light(p);
		
		Point3d pos1(10,20,30);
		Color3d color1(1,1,1);
		PointLight  * p1 = new PointLight(pos1,color1);
		scene.add_light(p1);
		
		
	}
	
	void defineGeometry()
	{
		Scene & scene = *this;
		
			/* define some colors */
		Color3d white(1.0);
		Color3d black(0.0);
		Color3d red(1,0,0.0);
		Color3d green(0,1.0,0.0);
		Color3d blue(0,0,1.0);
		
		
		Point3d center3(0,0,2);
		double radius3 = 0.25;
		Sphere * sp3 = new Sphere(center3,radius3);
		sp3->diffuse() = white * 0.1;
		sp3->transparency() = white * 0.9;
		sp3->specular() = white;
		sp3->shining() = 64;
		sp3->index() = 1.5;
		scene.add_object(sp3);
		
		
	}
	
	virtual void setDefaultCamera(Camera& camera) const
	{
		Point3d pos(0,0,-2);
		double fov_h = 50 / 180.0 * M_PI ;
		Point3d coi(0,0,1);
		Vector3d up(0,1,0) ;
		//		Point3d pos(0,8,35);
		//		double fov_h = 30 / 180.0 * M_PI;
		//		Point3d coi(0,0,-0);
		//		Vector3d up(0,1,0) ;
		camera = Camera(pos,coi,up,fov_h);
	}
	
	virtual ~Scene4() {

	}

};


#endif
