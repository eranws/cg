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
_vao(0), _vbo(0), _width(w), _height(h), _glPolygonMode(GL_FILL)
{
	for (int i=0; i<3; i++)
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
}



void Model::genModelVertices()
{

	//	std::vector<float> vertices;
	//	vertices.reserve(_mesh.n_vertices() * 4);
	//	// iterate on all faces
	//
	//	for (MyMesh::VertexIter v_it=_mesh.vertices_begin(); v_it != _mesh.vertices_end(); ++v_it)
	//	{
	//		vertices.push_back(_mesh.point(v_it.handle())[0]);
	//		vertices.push_back(_mesh.point(v_it.handle())[1]);
	//		vertices.push_back(_mesh.point(v_it.handle())[2]);
	//		vertices.push_back(1.0f);
	//	}
	//
	//	struct face_indices_t
	//	{
	//		face_indices_t() {
	//
	//		}
	//		GLuint a,b,c;
	//	};
	//	int i = 0;
	//
	//	std::vector<face_indices_t> faces(_mesh.n_faces());
	//	faces.reserve(_mesh.n_faces());
	//	for (MyMesh::FaceIter f_it = _mesh.faces_begin(); f_it != _mesh.faces_end(); ++f_it)
	//	{
	//		MyMesh::ConstFaceVertexIter cfv_it = _mesh.cfv_iter(f_it.handle());
	//		face_indices_t face;
	//		face.a = cfv_it.handle().idx();
	//		++cfv_it;
	//		face.b = cfv_it.handle().idx();
	//		++cfv_it;
	//		face.c = cfv_it.handle().idx();
	//		faces[i++] = face;
	//	}
	//
	//	glGenBuffers(1, &_ebo);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face_indices_t)*faces.size(), &(faces[0]), GL_STATIC_DRAW);


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
	std::cout <<"center of object "<< _center<< std::endl;
	std::cout <<"lower left corner of object "<< _lowerLeft << std::endl;
	std::cout <<"upper right corner of object "<< _upperRight << std::endl;


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
		computeCenterAndBoundingBox();
		genModelVertices();

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
	}

	resetMatrices();
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
	_fov = 35.0f;
	_fovBase = _fov;
	_projectionMat = glm::perspective(_fov, float(_width)/float(_height), OBJECT_DEPTH - OBJECT_B_RAD, OBJECT_DEPTH + OBJECT_B_RAD);
	_viewMat = glm::lookAt(glm::vec3(0, 0, OBJECT_DEPTH), glm::vec3(),glm::vec3(0,1,0));

	_translateMat = glm::mat4(1.0f);
	_rotationMat =  glm::mat4(1.0f);
	_rotateBaseMat = _rotationMat;

	_scale = 0.8f;
	_scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(_scale, _scale, _scale));

	_translateBase = glm::vec2(0,0);
}

void Model::draw()
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	glPolygonMode(GL_FRONT_AND_BACK, _glPolygonMode);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);

	glm::mat4 transform  = _projectionMat *  _viewMat * _rotationMat *  _translateMat * _scaleMat;
	glUniformMatrix4fv(_transformUV, 1, GL_FALSE, glm::value_ptr(transform));
	glDrawArrays(GL_TRIANGLES, 0, _mesh.n_faces() * 3);

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

glm::vec3 arcBall(glm::vec2 v)
{
	float z;
	if (glm::length(v) > 1)
	{
		z = 0;
	}
	else
	{
		z = std::sqrt(1 - v.x*v.x - v.y*v.y);
	}

	return glm::vec3(v.x, v.y, z);

}
void Model::rotate(int x, int y)
{
	glm::vec2 p1 = getScreenUnitCoordinates(glm::vec2(x, y));
	glm::vec2 p2 = getScreenUnitCoordinates(_xyRotate);

	glm::vec3 v1 = arcBall(p1);
	glm::vec3 v2 = arcBall(p2);

	glm::vec3 dir = glm::normalize(glm::cross(v2, v1));
	float s = glm::acos(glm::dot(v1, v2));
	s *= 100;

	_rotationMat = glm::rotate(_rotateBaseMat, s, dir);

}

void Model::scale(int y)
{
	int dy = _yScale - y;
	_fov = _fovBase * (1 + dy / _height);
	_projectionMat = glm::perspective(_fov, float(_width)/float(_height), OBJECT_DEPTH - OBJECT_B_RAD, OBJECT_DEPTH + OBJECT_B_RAD);
}

void Model::translate(int x, int y)
{
	glm::vec2 dxy = getScreenUnitCoordinates(_xyTranslate) - getScreenUnitCoordinates(glm::vec2(x, y));

	_translate = _translateBase - dxy;
	_translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(_translate, 0));
}


void Model::setFlag(int button, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		_xyRotate = glm::vec2(x, y);
		_mouseFlags[GLUT_LEFT_BUTTON] = true;
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		_yScale = y;
		_mouseFlags[GLUT_MIDDLE_BUTTON] = true;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		_xyTranslate = glm::vec2(x, y);
		_mouseFlags[GLUT_RIGHT_BUTTON] = true;
	}
}

void Model::resetFlag(int button)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		_rotateBaseMat = _rotationMat;
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		_fovBase = _fov;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		_translateBase = _translate;
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
