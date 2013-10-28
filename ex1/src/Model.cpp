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

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Ball.h"

#define SHADERS_DIR "shaders/"

Model::Model() :
_vao(0), _vbo(0),_numVertices(DEFAULT_VERTICES_NUM)
{

}

Model::~Model()
{
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	delete _vertices;
}


//Generate the circle vertices by assigning a (x,y) position for each vertex
//This function is called at init, and also at resize.
//When resizing the function allocates a new float array for the vertices,
//and deletes the old one.
void Model::genCircleVertices()
{
	float* tempVertices = new float[_numVertices * 4];

	const float step = 2 * PI / (_numVertices - 1);

	for(int i = 0; i < _numVertices; i++)
	{
		float angle = i * step;
		tempVertices[i * 4] = cos(angle);
		tempVertices[i * 4 + 1] = sin(angle);

		tempVertices[i * 4 + 2] = 0.0f;
		tempVertices[i * 4 + 3] = 1.00f;
	}


	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	glBufferData(GL_ARRAY_BUFFER, _numVertices * 4 * sizeof(float), tempVertices, GL_STATIC_DRAW);
	delete[] tempVertices;
}

void Model::init()
{
	programManager::sharedInstance()
	.createProgram("default",
			SHADERS_DIR "TranslateShader.vert",
			SHADERS_DIR "CheckeredShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");

	// Obtain uniform variable handles:
	_fillColorUV  = glGetUniformLocation(program, "fillColor");
	_uniTrans = glGetUniformLocation(program, "trans");

	// Initialize vertices buffer and transfer it to OpenGL
	{
		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//generate vertices to create the circle
		genCircleVertices();

		// Obtain attribute handles:
		_posAttrib = glGetAttribLocation(program, "position");
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

glm::vec2 Model::getScreenUnitCoordinates(glm::vec2 pos)
{
	glm::vec2 res = pos;
	res.x = 2 * (res.x / _width) - 1.f;
	res.y = 2 * (res.y / _height) - 1.f;
	res.y *= -1; // y axis is pointing upwards
	return res;

}


void Model::draw()
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	GLenum polygonMode = GL_FILL;   // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);
	for (size_t i=0; i<_balls.size();i++)
	{
		Ball& ball = _balls[i];
		glUniform4f(_fillColorUV, ball.color.r, ball.color.g, ball.color.b, 1.0);

		glm::mat4 sc = glm::scale(glm::mat4(), glm::vec3(2 * ball.radius / _width, 2 * ball.radius / _height, 0));
		glm::mat4 tr = glm::translate(glm::mat4(), glm::vec3(getScreenUnitCoordinates(ball.pos), 0));

		glm::mat4 transform = tr * sc;

		glUniformMatrix4fv(_uniTrans, 1, GL_FALSE, glm::value_ptr(transform));

		glDrawArrays(GL_TRIANGLE_FAN, 0, _numVertices);
	}

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
	//change number of vertices according to window size, minimum is one vertex, max is 50
	_numVertices = std::min(50, std::min(width, height));
	genCircleVertices();
}

void Model::mouse(int button, int state, int x, int y)
{
	glm::vec2 pos(x, y);
	bool isClear = true; //check if we didn't hit any other ball
	for (size_t i=0; i<_balls.size();i++)
	{
		float dist = glm::distance(pos, _balls[i].pos);
		if (dist <= _balls[i].radius)
		{
			isClear = false;
		}
	}

	if (isClear)
	{
		_balls.push_back(Ball(x,y,this));
	}

}

void Model::update()
{
	// update positions
	for (size_t i=0; i<_balls.size();i++)
	{
		_balls[i].update();
	}

	std::vector<float> minRads(_balls.size());
	// check for collisions
	for (size_t i=0; i<_balls.size();i++)
	{
		float minRadius = _balls[i].initialRadius;
		for (size_t j=0; j<_balls.size();j++)
		{
			if (i==j) continue;
			float dist = glm::distance(_balls[i].pos, _balls[j].pos);

			float tempRadius = minRadius;

			if (dist < _balls[i].initialRadius + _balls[j].initialRadius)
			{
				tempRadius = (dist - _balls[j].radius + _balls[i].radius) / 2;
			}

			// update radius
			if (tempRadius < minRadius)
			{
				minRadius = tempRadius;
			}
		}
		minRads[i] = minRadius;
	}

	for (size_t i=0; i<_balls.size();i++)
	{
		_balls[i].setRadius(minRads[i]);
	}




}