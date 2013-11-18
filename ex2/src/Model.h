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
#define LIGHT_SOURCE1 200.0, 200.0
#define LIGHT_SOURCE2 400.0, 400.0

class Ball;
class Model {

	GLuint _vao, _vbo, _ebo;

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

	glm::mat4 _projectionMat, _viewMat, _displaceMat, _rotationMat, _translateMat;
	float _fov;
	float _fovBase;


	int _yScale;
	float _scale;
	float _scaleBase;

	glm::vec2 _xyTranslate, _xyRotate;
	glm::vec2 _translate;
	glm::vec2 _translateBase;
	glm::mat4 _rotateBaseMat;

	static const float OBJECT_DEPTH = 5;
	static const float OBJECT_B_RAD = 3;
	bool _mouseFlags[3];

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

	void setFlag(int button, int x, int y);
	void resetFlag(int button);
	void motion(int x, int y);
};

#endif /* defined(__ex0__Model__) */
