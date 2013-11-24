//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <math.h>
#include <vector>
#include <glm/glm.hpp>

#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"

#define PI 3.14159265359
typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;

class Model {

public:
	enum shadingMode{
		SHADING_PHONG, SHADING_GOURAUD, SHADING_RGB
	};

private:


	//	float _modelScale;
	static const float INITIAL_FOV = 35.f;
	static const float MAX_FOV = 150.f;
	static const float MIN_FOV = 10.f;
	static const float CIRCLE_RADIUS = 0.8f;
	static const float MODEL_SCALE = 0.45;

	GLuint _vao, _vbo, _vaoCircle, _vboCircle;

	// Attribute handle:
	GLint _posAttrib, _posAttribCircle;
	GLint _transformUV;

	// View port frame:
	float _width, _height;

	GLenum _glPolygonMode;

	MyMesh _mesh;

	MyMesh::Point _center, _lowerLeft, _upperRight;

	glm::mat4 _projectionMat, _viewMat, _rotationMat, _translateMat, _modelMat, _scaleMat;

	float _fov, _fovBase;
	int _yScaleBase;
	glm::vec2 _xyTranslateBase, _xyRotateBase;

	static const float OBJECT_DEPTH = 3;
	static const float OBJECT_B_RAD = 2;

	bool _mouseFlags[3];

	enum viewMode{
		ORTHOGONAL, PERSPECTIVE
	};
	viewMode _viewMode;


	enum normalMode{
		NORMAL_FACE, NORMAL_VERTEX
	};
	normalMode _normalMode;

	shadingMode _shadingMode;

	struct LightParams {
		LightParams() :
			specExp(200.0), // Shininess (can be changed dynamically by the user)
			ambientColor(glm::vec3(1.0, 1.0, 1.0)),
			lightColor1(glm::vec3(1.0, 0.9, 0.7)), // Light 1 color
			lightColor2(glm::vec3(0.6, 0.6, 1.0)), // Light 2 color
			highColor(glm::vec3(1.0, 1.0, 1.0)), // Specular color
			ka(0.1), // Ambient color
			kd(glm::vec3(0.3, 0.3, 0.3)), // Specularity coefficient
			ks(0.3), // Diffuse
			light_position1(glm::vec3(  3.0, 2.0, 1.0)), // First light position
			light_position2(glm::vec3( -3.0, 0.0, 1.0)) // Second light position
		{
		}

		float specExp;
		glm::vec3 ambientColor;
		glm::vec3 lightColor1;
		glm::vec3 lightColor2;
		glm::vec3 highColor;
		float ka;
		glm::vec3 kd;
		float ks;
		glm::vec3 light_position1;
		glm::vec3 light_position2;
	};

	LightParams lightParams;


	int _numCircleVertices;


public:
	Model(float w, float h);

	virtual ~Model();

	void init(const char* meshFile);

	void draw();
	void draw2D();


	void resize(int width, int height);

	void genModelVertices();

	float getWidth() const {return _width;}
	float getHeight() const {return _height;}
	glm::vec2 getScreenUnitCoordinates(glm::vec2 pos);
	glm::vec2 normalizeScreenCoordninates(glm::vec2 v);

	void loadMesh(const char* fileName);

	void computeCenterAndBoundingBox();

	void changePolygonMode();

	void resetMatrices();

	void scale(int y);
	void translate(int x, int y);
	void rotate(int x, int y);
	glm::vec3 arcBall(glm::vec2 v);

	void setMouseFlag(int button, int x, int y);
	void resetMouseFlag(int button);
	void motion(int x, int y);

	void toggleProjectionMode();
	void updateProjectionMatrix();

	void genCircleVertices();

	void computeFaceNormals();

	void decreaseShine(){} //TODO
	void increaseShine(){} //TODO

	void setShadingMode(shadingMode mode){_shadingMode = mode;}
	void toggleNormalMode();

};

#endif /* defined(__ex0__Model__) */
