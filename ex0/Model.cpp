//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define SHADERS_DIR "shaders/"

Model::Model() :
_vao(0), _vbo(0), _program(0), numberOfVertices(100)
{

}

Model::~Model()
{
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
}

void Model::loadData()
{
	// Initialize vertices buffer and transfer it to OpenGL
		{
			const int precision = numberOfVertices;

			const float PI = 3.14159265358979323846;
			float step = 2 * PI / precision;
			float* vertices = new float[precision * 4];
			const float relativeSize = 0.6; // circle size relative to the window

			for(unsigned int i = 0; i < precision; ++i)
			{
				vertices[i*4 + 0] = cosf(i * step);
				vertices[i*4 + 1] = sinf(i * step);
				vertices[i*4 + 0] = cosf(i * step) * relativeSize;
				vertices[i*4 + 1] = sinf(i * step) * relativeSize;

				vertices[i*4 + 2] = 0.0f;
				vertices[i*4 + 3] = 1.0f;
			}

			// Create and bind the object's Vertex Array Object:
			glGenVertexArrays(1, &_vao);
			glBindVertexArray(_vao);

			// Create and load vertex data into a Vertex Buffer Object:
			glGenBuffers(1, &_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * precision * 4, vertices, GL_STATIC_DRAW);

			// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:

			// Obtain attribute handles:
			_posAttrib = glGetAttribLocation(_program, "position");
			glEnableVertexAttribArray(_posAttrib);
			glVertexAttribPointer(_posAttrib, // attribute handle
								  4,          // number of scalars per vertex
								  GL_FLOAT,   // scalar type
								  GL_FALSE,
								  0,
								  0);

			// Unbind vertex array:
			glBindVertexArray(0);
		}
}

void Model::init()
{
	programManager::sharedInstance()
	.createProgram("default",
				   SHADERS_DIR "ChekersShader.vert",
				   SHADERS_DIR "ChekersShader.frag");

	_program = programManager::sharedInstance().programWithID("default");
		
	// Obtain uniform variable handles:
	_fillColorUV  = glGetUniformLocation(_program, "fillColor");

	loadData();
}

void Model::draw()
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	GLenum polygonMode = GL_FILL;   // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// Set uniform variable with RGB values:
	float red = 0.3f; float green = 0.5f; float blue = 0.7f;
	glUniform4f(_fillColorUV, red, green, blue, 1.0);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices); //for some reason GL_POLYGON didn't work...
	
	// Unbind the Vertex Array object
	glBindVertexArray(0);
	
	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::resize(int width, int height)
{
    _width	= width;
    _height = height;
    _offsetX = 0;
    _offsetY = 0;
}
