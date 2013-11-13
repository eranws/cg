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

#define SHADERS_DIR "shaders/"

Model::Model(float w, float h) :
_vao(0), _vbo(0), _width(w), _height(h)
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



void Model::genCircleVertices()
{

	std::vector<float> temp;
	temp.reserve(_mesh.n_faces() * 3 * 4);
	// iterate on all faces
	for (MyMesh::FaceIter h_it=_mesh.faces_begin(); h_it!=_mesh.faces_end(); ++h_it)
	{
		// circulate around the current face
		for (MyMesh::FaceVertexIter fv_it = _mesh.fv_iter(h_it); fv_it; ++fv_it)
		{
			temp.push_back(_mesh.point(fv_it.handle())[0]);
			temp.push_back(_mesh.point(fv_it.handle())[1]);
			temp.push_back(_mesh.point(fv_it.handle())[2]);
			temp.push_back(1.0f);
		}
	}

	//printf("vao size: %d\n", temp.size());

/*
	temp.clear();
	for (int i=0; i<_mesh.n_faces(); i++)
	{
		temp.push_back(0.0);
		temp.push_back(0.0);
		temp.push_back(0.5);
		temp.push_back(1.0);

		temp.push_back(0.0);
		temp.push_back(0.5);
		temp.push_back(0.5);
		temp.push_back(1.0);

		temp.push_back(0.5);
		temp.push_back(0.0);
		temp.push_back(0.5);
		temp.push_back(1.0);
	}
*/



	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	glBufferData(GL_ARRAY_BUFFER, temp.size() * sizeof(float), temp.data(), GL_STATIC_DRAW);
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

	//_projectionUV = glGetUniformLocation(program, "projection");




	// Initialize vertices buffer and transfer it to OpenGL
	{
		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//generate vertices to create the circle

		loadMesh(meshFile);
		genCircleVertices();
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




//	glm::lookAt(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	_x = _y = 0;



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


	glm::mat4 projection =
			glm::perspective(45.0f, 1.0f, 0.1f, 100.f);
//glm::perspective(50, 1, 1, 10);

	glm::mat4 sc = glm::scale(glm::mat4(), glm::vec3(0.1, 0.1, 0.1));

//	glm::mat4 tr = glm::translate(glm::mat4(), glm::vec3(1, 2 * (_y / _height - 0.5), 10 + 5 * (_x / _width - 0.5) ));

	glm::mat4 tr = glm::translate(glm::mat4(), glm::vec3(0, 0, 5 * (_x / _width - 0.5) ));

	glm::mat4 transform = projection * tr * sc;
	glUniformMatrix4fv(_transformUV, 1, GL_FALSE, glm::value_ptr(transform));

	//glUniformMatrix4fv(_projectionUV, 1, GL_FALSE, glm::value_ptr(projection));


	//
	//		glUniform4f(_fillColorUV, ball._color.r, ball._color.g, ball._color.b, 1.0);
	//		glUniform2f(_centerUV, ball._pos.x, ball._pos.y);
	//		glUniform1f(_radiusUV, ball._radius);
	//		glUniform2f(_lightSource1UV, LIGHT_SOURCE1);
	//		glUniform2f(_lightSource1UV, LIGHT_SOURCE2);
	//
	//
	glDrawArrays(GL_TRIANGLES, 0, _mesh.n_faces() * 3);
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
}

void Model::mouse(int button, int state, int x, int y)
{
	_x = x;
	_y = y;

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
