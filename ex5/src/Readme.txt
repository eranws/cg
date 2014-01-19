eranws 300489960
ilansh 302514401


Implementation:
---

Our example commands are given in the makefile.
To run each scene type: make sceneX (1 <= X <= 5)
The parameters for scene 1: ex5 2 3 0.7 0
The parameters for scene 2: ex5 1 3 1.3 1
The parameters for scene 3: ex5 1 1 1 2

Sphere lights:
We implemented sphere lights simply by adding a radius member to the PointLight class.
A PointLight is instantiated from this class with radius EPSILON.
We implemented shadow rays by defining 6 fixed point on the sphere 
(corresponding with the 6 touching points of a bounding box), and added more random shadow rays 
as a function of the light radius - This will make results more accurate, since the larger the light is,
The bigger the chance of it reaching a point.
The shadow rays are then shot from the light source in the direction of the current rendered pixel,
If a ray reaches it without hitting any object on the way - we count this pixel as "illuminated" and break 
(no need to check for more rays).


Our parametric object:
We implemented a Lens defined by the intersection of two spheres of the same size.
We created two scenes with our lens object:
Scene4: A model of Glasses with an ellipsoid behind. (to run in type "make scene4")
Scene5: We showcase the magnifying ability of our lens by replacing the front sphere in scene2 with our lens. (to run it type "make scene5")

The lens was difficult to implement correctly, and due to its importance in optics and photography, we feel that a small bonus might be appropriate (especially since we submitted without an extension).

Thanks (:


Files sent:
---
Readme.txt - this file
Makefile

# list of all the sources files
SRCS	=  	bimage.cpp      \
            ex5.cpp         \
            scene.cpp       \
            triangle.cpp    \
            camera.cpp      \
            object.cpp      \
            polygon.cpp     \
            MyMeshObject.cpp\
            Lens.cpp        \
            Ellipsoid.cpp   \
              

# list of all the headers files
HDRS	=  	camera.h        \
            general.h       \
            lights.h        \
            MyMesh.h        \
            MyMeshObject.h  \
            MyVecs.h        \
            object.h        \
            polygon.h       \
            profiling_block_timer.h \
            ray.h           \
            Scene1.h        \
            Scene2.h        \
            Scene3.h        \
            SceneChooser.h  \
            scene.h         \
            sphere.h        \
            triangle.h      \
            Lens.h          \
            Ellipsoid.h     \
            bimage.h        \
            Scene4.h        \
            Scene5.h

Texture Files:
textures/checkerboard_lg.bmp
textures/warning.bmp
textures/wood.bmp



Mesh files:
Meshes/girl_face.obj

website used:
---
http://open.gl/
