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
_vao(0), _vbo(0), _vaoCircle(0), _vboCircle(0),
_width(w), _height(h), _glPolygonMode(GL_FILL), _viewMode(PERSPECTIVE), _numCircleVertices(50)
{
	for (int i=0; i < 3; i++)
	{
		_mouseFlags[i] = false;
	}
}

Model::~Model()
{
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_vaoCircle != 0)
		glDeleteVertexArrays(1, &_vaoCircle);
	if (_vboCircle != 0)
		glDeleteBuffers(1, &_vboCircle);
}



void Model::genModelVertices()
{
	std::vector<float> temp;
	temp.reserve(_mesh.n_faces() * 3 * 4);

	std::vector<float> v;
	v.push_back(fabs(_lowerLeft[0]));
	v.push_back(fabs(_lowerLeft[1]));
	v.push_back(fabs(_lowerLeft[2]));

	v.push_back(fabs(_upperRight[0]));
	v.push_back(fabs(_upperRight[1]));
	v.push_back(fabs(_upperRight[2]));

	float maxV = *std::max_element(v.begin(), v.end());

	for (MyMesh::FaceIter h_it=_mesh.faces_begin(); h_it!=_mesh.faces_end(); ++h_it)
	{
		// circulate around the current face
		for (MyMesh::FaceVertexIter fv_it = _mesh.fv_iter(h_it); fv_it; ++fv_it)
		{
			MyMesh::Point p = _mesh.point(fv_it.handle());
			// normalize each point
			p -= _center; // center of mass
			p /= maxV;

			temp.push_back(p[0]);
			temp.push_back(p[1]);
			temp.push_back(p[2]);
			temp.push_back(1.0f);
		}
	}

	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	glBufferData(GL_ARRAY_BUFFER, temp.size() * sizeof(float), temp.data(), GL_STATIC_DRAW);
}

void Model::genCircleVertices()
{
	float* tempVertices = new float[_numCircleVertices * 2 * 4];

	const float step = 2 * PI / (_numCircleVertices - 1);

	for(int i = 0; i < _numCircleVertices * 2; i++)
	{
		float angle = i * step;
		tempVertices[i * 4] = cosf(angle) * CIRCLE_RADIUS;
		tempVertices[i * 4 + 1] = sinf(angle) * CIRCLE_RADIUS;

		tempVertices[i * 4 + 2] = 0;
		tempVertices[i * 4 + 3] = 1.00f;
	}


	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	glBufferData(GL_ARRAY_BUFFER, _numCircleVertices * 2 * 4 * sizeof(float), tempVertices, GL_STATIC_DRAW);
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
	MyMesh::Point p(0,0,0);
	const float fm = std::numeric_limits<float>::max();
	_lowerLeft = MyMesh::Point(fm, fm, fm);
	_upperRight = MyMesh::Point(0,0,0);
	/* number of vertices in the _mesh */
	int vNum = _mesh.n_vertices();
	vertexIter = _mesh.vertices_begin();
	_lowerLeft = _upperRight = _mesh.point(vertexIter);
	/* This is how to go over all the vertices in the _mesh */
	for (vertexIter = _mesh.vertices_begin(); vertexIter != _mesh.vertices_end(); ++vertexIter){
		/* this is how to get the point associated with the vertex */
		p = _mesh.point(vertexIter);
		_center += p;
		for (int i = 0; i < 3; i++) {
			_lowerLeft[i] = std::min(_lowerLeft[i], p[i]);
			_upperRight[i] = std::max(_upperRight[i], p[i]);
		}
	}
	_center /= (double)vNum;

//	std::vector<float> v;
//	v.push_back(fabs(_lowerLeft[0]));
//	v.push_back(fabs(_lowerLeft[1]));
//	v.push_back(fabs(_lowerLeft[2]));
//
//	v.push_back(fabs(_upperRight[0]));
//	v.push_back(fabs(_upperRight[1]));
//	v.push_back(fabs(_upperRight[2]));
//
//	_modelScale = CIRCLE_RADIUS / (*std::max_element(v.begin(), v.end()));
}


void Model::loadMesh(const char* fileName) {
	if (!OpenMesh::IO::read_mesh(_mesh, fileName))
	{
		// if we didn't make it, exit...
		fprintf(stderr, "Error loading mesh, Aborting.\n");
	}
}

void Model::init(const char* meshFile)
{
	programManager::sharedInstance()
	.createProgram("default",
			SHADERS_DIR "ModelShader.vert",
			SHADERS_DIR "ModelShader.frag");


	programManager::sharedInstance()
		.createProgram("circle",
			SHADERS_DIR "CircleShader.vert",
			SHADERS_DIR "CircleShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");
	GLuint programCircle = programManager::sharedInstance().programWithID("circle");





	// Initialize vertices buffer and transfer it to OpenGL

	// Initialize Circle
	{
		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vaoCircle);
		glBindVertexArray(_vaoCircle);

		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vboCircle);
		glBindBuffer(GL_ARRAY_BUFFER, _vboCircle);

		//generate vertices to create the circle
		genCircleVertices();

		// Obtain attribute handles:
		_posAttribCircle = glGetAttribLocation(programCircle, "position");
		glEnableVertexAttribArray(_posAttribCircle);
		glVertexAttribPointer(_posAttribCircle, // attribute handle
				4,          // number of scalars per vertex
				GL_FLOAT,   // scalar type
				GL_FALSE,
				0,
				0);

		glBindVertexArray(0);
	}

	// Initialize Model
	{
		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//generate vertices to create the circle

		loadMesh(meshFile);
		computeCenterAndBoundingBox();
		genModelVertices();


		// Obtain uniform variable handles:
		_transformUV = glGetUniformLocation(program, "transform");
		_lowerLeftUV = glGetUniformLocation(program, "lowerLeft");
		_upperRightUV = glGetUniformLocation(program, "upperRight");

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

		resetMatrices();
	}


}

glm::vec2 Model::getScreenUnitCoordinates(glm::vec2 pos)
{
	pos.x = 2 * (pos.x / _width) - 1.f;
	pos.y = 2 * (pos.y / _height) - 1.f;
	pos.y = -pos.y;
	return pos;

}

void Model::resetMatrices()
{
	_fov = INITIAL_FOV;
	_fovBase = _fov;
	_projectionMat = glm::perspective(_fov, float(_width)/float(_height), OBJECT_DEPTH - OBJECT_B_RAD, OBJECT_DEPTH + OBJECT_B_RAD);
	_viewMat = glm::lookAt(glm::vec3(0, 0, OBJECT_DEPTH), glm::vec3(),glm::vec3(0,1,0));
	_modelMat = glm::mat4();
	_translateMat = glm::mat4();
	_rotationMat =  glm::mat4();

	_scaleMat = glm::scale(glm::mat4(), glm::vec3(MODEL_SCALE));
}

void Model::toggleProjectionMode()
{
	if(_viewMode == ORTHOGONAL)
	{
		_viewMode = PERSPECTIVE;
	}
	else
	{
		_viewMode = ORTHOGONAL;
	}
	updateProjectionMatrix();

}

void Model::updateProjectionMatrix()
{
	if(_viewMode == PERSPECTIVE)
	{
		_projectionMat = glm::perspective(_fov, float(_width)/float(_height), OBJECT_DEPTH - OBJECT_B_RAD, OBJECT_DEPTH + OBJECT_B_RAD);
	}
	else
	{
		float ty = tanf(glm::radians(_fov / 2)) * 2;
		float tx = ty * float(_width)/float(_height);

		_projectionMat = glm::ortho(-tx, tx, -ty, ty, OBJECT_DEPTH - OBJECT_B_RAD, OBJECT_DEPTH + OBJECT_B_RAD);
	}
}

void Model::draw()
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	glPolygonMode(GL_FRONT_AND_BACK, _glPolygonMode);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);

	glm::mat4 transform  = _projectionMat *  _translateMat * _viewMat  * _rotationMat * _modelMat * _scaleMat;


	glUniformMatrix4fv(_transformUV, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform3f(_lowerLeftUV, _lowerLeft[0], _lowerLeft[1], _lowerLeft[2]);
	glUniform3f(_upperRightUV, _upperRight[0], _upperRight[1], _upperRight[2]);
	glDrawArrays(GL_TRIANGLES, 0, _mesh.n_faces() * 3);

	// Unbind the Vertex Array object
	glBindVertexArray(0);

	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::draw2D()
{

	GLuint program = programManager::sharedInstance().programWithID("circle");
		glUseProgram(program);

	glBindVertexArray(_vaoCircle);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_LINES, 0, _numCircleVertices * 4);

	// Unbind the Vertex Array object
	glBindVertexArray(0);

}


void Model::resize(int width, int height)
{
	_width	= width;
	_height = height;
	updateProjectionMatrix();
}

void Model::changePolygonMode()
{
	if(_glPolygonMode == GL_FILL)
	{
		_glPolygonMode = GL_LINE;
	}
	else
	{
		_glPolygonMode = GL_FILL;
	}
}


glm::vec2 Model::normalizeScreenCoordninates(glm::vec2 v)
{
	v.x /= _width;
	v.y /= _height;

	return v;
}

glm::vec3 Model::arcBall(glm::vec2 v)
{
	float z;
	if (glm::length(v) > 1)
	{
		z = 0;
	}
	else
	{
		z = std::sqrt(1 - glm::dot(v,v));
	}

	return glm::vec3(v.x, v.y, z);
}


void Model::rotate(int x, int y)
{
	glm::vec2 p1 = getScreenUnitCoordinates(_xyRotateBase);
	glm::vec2 p2 = getScreenUnitCoordinates(glm::vec2(x,y));

	p1 *= (1/CIRCLE_RADIUS);
	p2 *= (1/CIRCLE_RADIUS);


	glm::vec3 v1 = arcBall(p1);
	glm::vec3 v2 = arcBall(p2);

	glm::vec3 dir = glm::normalize(glm::cross(v1, v2));
	float angle =  2 * glm::degrees(glm::acos(glm::dot(glm::normalize(v1), glm::normalize(v2))));

	_rotationMat = glm::rotate(glm::mat4(), angle, dir);
}

void Model::scale(int y)
{
	int dy = _yScaleBase - y;
	_fov = _fovBase * (1 + dy / _height);
	if (_fov > MAX_FOV)
	{
		_fov = MAX_FOV;
	}
	if (_fov < MIN_FOV)
	{
		_fov = MIN_FOV;
	}

	updateProjectionMatrix();
}

void Model::translate(int x, int y)
{
	glm::vec2 dxy = getScreenUnitCoordinates(glm::vec2(x, y)) - getScreenUnitCoordinates(_xyTranslateBase);
	_translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(dxy, 0));
	//	_center += MyMesh::Point(x,y);
}


void Model::setMouseFlag(int button, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		_xyRotateBase = glm::vec2(x, y);
		if (glm::length(getScreenUnitCoordinates(_xyRotateBase)) < CIRCLE_RADIUS)
		{
			_mouseFlags[GLUT_LEFT_BUTTON] = true;
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		_yScaleBase = y;
		_mouseFlags[GLUT_MIDDLE_BUTTON] = true;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		_xyTranslateBase = glm::vec2(x, y);
		_mouseFlags[GLUT_RIGHT_BUTTON] = true;
	}
}

void Model::resetMouseFlag(int button)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		_modelMat = _rotationMat * _modelMat;
		_rotationMat = glm::mat4();
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		_fovBase = _fov;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		_modelMat = _translateMat * _modelMat;
		_translateMat = glm::mat4();
	}

	_mouseFlags[button] = false;
}

void Model::motion(int x, int y)
{
	if (_mouseFlags[GLUT_LEFT_BUTTON])
	{
		rotate(x, y);
	}
	else if (_mouseFlags[GLUT_MIDDLE_BUTTON])
	{
		scale(y);
	}
	else if (_mouseFlags[GLUT_RIGHT_BUTTON])
	{
		translate(x, y);
	}
}
