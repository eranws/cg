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
#include "Ball.h"


#define DEFAULT_VERTICES_NUM 40
#define PI 3.14159265

class Ball;
class Model {
	
	GLuint _vao, _vbo;

	// Attribute handle:
	GLint _posAttrib;
	
	// Uniform handle:
	GLint _fillColorUV;
	GLint _uniTrans;
	
	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	//circle vertices, including center and perimeter vertices.
	float* _vertices;

	//number of vertices in the circle, not including center and last perimeter vertex (i.e total number of vertices - 2)
	int _numVertices;

	//Balls array
	std::vector <Ball> _balls;
	friend class Ball;

public:
	Model();

	virtual ~Model();

	void init();
	
	void update();

	void draw();
	
	void resize(int width, int height);
	
	void genCircleVertices();

	void mouse(int button, int state, int x, int y);

	float getWidth() const {return _width;}
	float getHeight() const {return _height;}
	glm::vec2 getScreenUnitCoordinates(glm::vec2 pos);

};

#endif /* defined(__ex0__Model__) */
