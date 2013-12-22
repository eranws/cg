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

class Model {
	
	GLuint _vao, _vbo, _ebo;

	// Attribute handles:
	GLint _posAttrib;
	
	// Uniform handle:
	GLint _fillColorUV;
	
	// Texture handles:
	GLuint _texture, _specMap;
	
	// View port frame:
	float _width, _height, _offsetX, _offsetY;
	
	size_t _numTriangles;

	struct face_indices_t
	{
		GLuint a,b,c;
	};

public:
	Model();

public:
	virtual ~Model();

public:
	void init();
	
public:
	void draw();
	
public:
	void resize(int width, int height);
	
};

#endif /* defined(__ex0__Model__) */
