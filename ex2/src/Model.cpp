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


#include <stdlib.h>

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

//#include "Ball.h"



#define SHADERS_DIR "shaders/"

Model::Model(float w, float h) :
_vao(0), _vbo(0),_numVertices(DEFAULT_VERTICES_NUM), _width(w), _height(h)
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


/** This function computes the geometrical center and the axis aligned bounding box of the
object. The bounding box is represented by the lower left and upper right corners. */
void Model::computeCenterAndBoundingBox()
{
	/* Vertex iterator is an iterator which goes over all the vertices of the _mesh */
	MyMesh::VertexIter vertexIter;
	/* This is the specific class used to store the geometrical position of the vertices of
	the mesh */
	MyMesh::Point p(0,0,0), center(0,0,0);
	const float fm = std::numeric_limits<float>::max();
	MyMesh::Point lowerLeft(fm, fm, fm);
	MyMesh::Point upperRight(0,0,0);
	/* number of vertices in the _mesh */
	int vNum = _mesh.n_vertices();
	vertexIter = _mesh.vertices_begin();
	lowerLeft = upperRight = _mesh.point(vertexIter);
	/* This is how to go over all the vertices in the _mesh */
	for (vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter){
		/* this is how to get the point associated with the vertex */
		p = _mesh.point(vertexIter);
		center += p;
		for (int i = 0; i < 3; i++) {
			lowerLeft[i] = std::min(lowerLeft[i], p[i]);
			upperRight[i] = std::max(upperRight[i], p[i]);
		}
	}
	center /= (double)vNum;
	std::cout <<"center of object "<< center<< std::endl;
	std::cout <<"lower left corner of object "<< lowerLeft<<std::endl;
	std::cout <<"upper right corner of object "<< upperRight<<std::endl;

}


/**
This function count how many vertices a face has
*/
void Model::faceValenceCounter()
{
	/* Face iterator */
	MyMesh::FaceIter fIter;
	/*In order to get information about a certain face (or edge or vertex) you need to get
	the handle for this entity. */
	/* once you have this handle you get ask the mesh object to give you information or to
	do something (nice) for you.*/
	MyMesh::FaceHandle faceH;
	for (fIter = _mesh.faces_begin();fIter != _mesh.faces_end(); ++fIter)
	{
		/* In this case we use the handle to get the index of the face and the number of
		vertices that this face has. */
		faceH = fIter.handle();
		std::cout <<"face #"<<faceH.idx()<<" has "<< _mesh.valence(faceH)<<" vertices"<< std::endl;
	}
}




void Model::loadMesh(const char* fileName) {
	if (!OpenMesh::IO::read_mesh(_mesh, fileName))
	{
		// if we didn't make it, exit...
		fprintf(stderr, "Error loading mesh, Aborting.\n");
	}
	printf("number of vertices is %d\n", _mesh.n_vertices());
	printf("number of faces is %d\n", _mesh.n_faces());
	printf("number of edges is %d\n", _mesh.n_edges());

}

void Model::init(const char* meshFile)
{
	programManager::sharedInstance()
	.createProgram("default",
			SHADERS_DIR "TranslateShader.vert",
			SHADERS_DIR "LightShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");

	// Obtain uniform variable handles:
	_fillColorUV  = glGetUniformLocation(program, "fillColor");
	_transformUV = glGetUniformLocation(program, "transform");
	_centerUV  = glGetUniformLocation(program, "ballCenter");
	_lightSource1UV  = glGetUniformLocation(program, "lightSource1");
	_lightSource2UV  = glGetUniformLocation(program, "lightSource2");
	_radiusUV = glGetUniformLocation(program, "radius");

	// Initialize vertices buffer and transfer it to OpenGL
	{
		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//generate vertices to create the circle
//		genCircleVertices();
		loadMesh(meshFile);
		computeCenterAndBoundingBox();
//		faceValenceCounter();

		// Obtain attribute handles:
		_posAttrib = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(_posAttrib);
		glVertexAttribPointer(_posAttrib, // attribute handle
				4,          // number of scalars per vertex
				GL_FLOAT,   // scalar type
				GL_FALSE,
				0,
				0);

		glBindVertexArray(0);
		mouse(GLUT_LEFT_BUTTON, GLUT_DOWN, _width / 2, _height / 2);
	}
}

glm::vec2 Model::getScreenUnitCoordinates(glm::vec2 pos)
{
	glm::vec2 res = pos;
	res.x = 2 * (res.x / _width) - 1.f;
	res.y = 2 * (res.y / _height) - 1.f;
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
//	for (size_t i=0; i<_balls.size();i++)
//	{
//		Ball& ball = _balls[i];
//
//		glm::mat4 sc = glm::scale(glm::mat4(), glm::vec3(2 * ball._radius / _width, 2 * ball._radius / _height, 0));
//		glm::mat4 tr = glm::translate(glm::mat4(), glm::vec3(getScreenUnitCoordinates(ball._pos), 0));
//
//		glm::mat4 transform = tr * sc;
//
//		glUniformMatrix4fv(_transformUV, 1, GL_FALSE, glm::value_ptr(transform));
//
//		glUniform4f(_fillColorUV, ball._color.r, ball._color.g, ball._color.b, 1.0);
//		glUniform2f(_centerUV, ball._pos.x, ball._pos.y);
//		glUniform1f(_radiusUV, ball._radius);
//		glUniform2f(_lightSource1UV, LIGHT_SOURCE1);
//		glUniform2f(_lightSource1UV, LIGHT_SOURCE2);
//
//
//		glDrawArrays(GL_TRIANGLE_FAN, 0, _numVertices);
//	}

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

//
}

void Model::mouse(int button, int state, int x, int y)
{
//	glm::vec2 pos(x, _height - y);
//	bool isClear = true; //check if we didn't hit any other ball
//	for (size_t i=0; i<_balls.size();i++)
//	{
//		float dist = glm::distance(pos, _balls[i]._pos);
//		if (dist <= _balls[i]._radius)
//		{
//			isClear = false;
//		}
//	}
//
//	if (isClear)
//	{
////		_balls.push_back(Ball(pos.x, pos.y, this));
//	}

}

void Model::update()
{
//	// update positions
//	for (size_t i=0; i<_balls.size();i++)
//	{
//		_balls[i].update();
//	}
//
//	std::vector<float> minRads(_balls.size());
//	// check for collisions
//	for (size_t i=0; i<_balls.size();i++)
//	{
//		float minRadius = _balls[i]._initialRadius;
//		for (size_t j=0; j<_balls.size();j++)
//		{
//			if (i==j) continue;
//			float dist = glm::distance(_balls[i]._pos, _balls[j]._pos);
//
//			float tempRadius = minRadius;
//
//			if (dist < _balls[i]._initialRadius + _balls[j]._initialRadius)
//			{
//				tempRadius = (dist - _balls[j]._radius + _balls[i]._radius) / 2;
//			}
//
//			// update radius
//			if (tempRadius < minRadius)
//			{
//				minRadius = tempRadius;
//			}
//		}
//		minRads[i] = std::max(minRadius, 1.0f);
//	}
//
//	for (size_t i=0; i<_balls.size();i++)
//	{
//		_balls[i].setRadius(minRads[i]);
//	}
}
