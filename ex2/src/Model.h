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


#define DEFAULT_VERTICES_NUM 40
#define PI 3.14159265

class Ball;
class Model {

	GLuint _vao, _vbo, _vao2, _vbo2;

	// Attribute handle:
	GLint _posAttrib;

	// Uniform handle:
	GLint _fillColorUV;
	GLint _transformUV;
	GLint _centerUV;
	GLint _lightSource1UV;
	GLint _lightSource2UV;
	GLint _radiusUV;

	GLint _projectionUV;


	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	GLenum _glPolygonMode;

	//circle vertices, including center and perimeter vertices.
	float* _vertices;

	typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;

	MyMesh _mesh;

	float _x, _y;

	MyMesh::Point _center, _lowerLeft, _upperRight;

	static const float MODEL_SCALE = 0.45f;
	glm::mat4 _projectionMat, _viewMat, _rotationMat, _translateMat, _modelMat, _scaleMat;

	static const float CIRCLE_RADIUS = 0.8f;
	float _fov;
	float _fovBase;

	int _yScale;
	glm::vec2 _xyTranslate, _xyRotate;

	static const float OBJECT_DEPTH = 3;
	static const float OBJECT_B_RAD = 2;
	bool _mouseFlags[3];

	enum viewMode{
		ORTHOGONAL, PERSPECTIVE
	};

	viewMode _viewMode;

	int _numCircleVertices;


public:
	Model(float w, float h);

	virtual ~Model();

	void init(const char* meshFile);

	void draw();

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

};

#endif /* defined(__ex0__Model__) */
