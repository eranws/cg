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
_width(w), _height(h), _glPolygonMode(GL_FILL), _viewMode(PERSPECTIVE),
_normalMode(NORMAL_FACE), _shadingMode(SHADING_PHONG), _numCircleVertices(50)
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
	if (_ebo != 0)
		glDeleteBuffers(1, &_ebo);
}



void Model::genModelVertices()
{

	std::vector<float> v;
	v.push_back(fabs(_lowerLeft[0]));
	v.push_back(fabs(_lowerLeft[1]));
	v.push_back(fabs(_lowerLeft[2]));

	v.push_back(fabs(_upperRight[0]));
	v.push_back(fabs(_upperRight[1]));
	v.push_back(fabs(_upperRight[2]));



	float maxV = *std::max_element(v.begin(), v.end());


	std::vector<glm::vec4> vertices(_mesh.n_vertices() * 2);

	_mesh.request_vertex_normals();
	_mesh.update_normals();

	size_t i = 0;
	MyMesh::Point p;
	for (MyMesh::VertexIter vertexIter = _mesh.vertices_begin();
			vertexIter != _mesh.vertices_end();
			++vertexIter)
	{
		p = _mesh.point(vertexIter.handle());
		glm::vec4 position((p[0] - _center[0]) / maxV, (p[1] - _center[1]) / maxV, (p[2] - _center[2]) / maxV, 1.0f);
		vertices[i++] = position;
		MyMesh::Normal n = _mesh.normal(vertexIter.handle());
		glm::vec4 normal(n[0], n[1], n[2], 0.0);
		vertices[i++] = normal;
	}

	_mesh.release_face_normals();

	// Iterate over faces and create a traingle for each face by referencing
	// to its vertices:
	std::vector<face_indices_t> faces(_mesh.n_faces());
	i = 0;
	for (MyMesh::FaceIter faceIter = _mesh.faces_begin();
			faceIter != _mesh.faces_end();
			++faceIter)
	{
		MyMesh::ConstFaceVertexIter cfvlt = _mesh.cfv_iter(faceIter.handle());
		face_indices_t face;
		face.a = cfvlt.handle().idx();
		++cfvlt;
		face.b = cfvlt.handle().idx();
		++cfvlt;
		face.c = cfvlt.handle().idx();
		faces[i++] = face;
	}

	{
		//		normalize points to arcBall radius
		_lowerLeft /= maxV;
		_lowerLeft *= CIRCLE_RADIUS;
		_upperRight /= maxV;
		_upperRight *= CIRCLE_RADIUS;
		float upperRightOffset = glm::length(glm::vec3(CIRCLE_RADIUS - _upperRight[0], CIRCLE_RADIUS - _upperRight[1], CIRCLE_RADIUS - _upperRight[2]));
		float lowerLeftOffset = glm::length(glm::vec3(CIRCLE_RADIUS + _lowerLeft[0], CIRCLE_RADIUS + _lowerLeft[1], CIRCLE_RADIUS + _lowerLeft[2]));
		float arcballNormalizeOffset = std::max(upperRightOffset, lowerLeftOffset);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i] *= (1 + arcballNormalizeOffset);
			vertices[i][3] = 1.0;
		}
	}

	// Create and load vertex data into a Vertex Buffer Object:
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);

	// Create and load face (elements) data into an Element Buffer Object:
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face_indices_t)*faces.size(), &(faces[0]), GL_STATIC_DRAW);
}

void Model::genCircleVertices()
{
	float* tempVertices = new float[_numCircleVertices * 4];

	const float step = (2.0 * PI) / (_numCircleVertices - 1);

	for(int i = 0; i < _numCircleVertices; i++)
	{
		float angle = i * step;
		tempVertices[i * 4] = cosf(angle) * CIRCLE_RADIUS;
		tempVertices[i * 4 + 1] = sinf(angle) * CIRCLE_RADIUS;

		tempVertices[i * 4 + 2] = 0;
		tempVertices[i * 4 + 3] = 1.f;
	}

	// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
	glBufferData(GL_ARRAY_BUFFER, _numCircleVertices * 4 * sizeof(float), tempVertices, GL_STATIC_DRAW);
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
}


void Model::loadMesh(const char* fileName) {
	if (!OpenMesh::IO::read_mesh(_mesh, fileName))
	{
		// if we didn't make it, exit...
		fprintf(stderr, "Error loading mesh, Aborting.\n");
	}
}

void Model::computeFaceNormals()
{
	//	_mesh.request_face_normals();
	//	_mesh.request_vertex_normals();
	//	_mesh.update_normals();
	//
	//	std::vector<float> faceNormals;
	//
	//	for (MyMesh::FaceIter h_it=_mesh.faces_begin(); h_it!=_mesh.faces_end(); ++h_it)
	//	{
	//		// circulate around the current face
	//		glm::vec3 face_normal = _mesh.normal(h_it);
	//		for (MyMesh::FaceVertexIter fv_it = _mesh.fv_iter(h_it); fv_it; ++fv_it)
	//		{
	//			MyMesh::Point p = _mesh.point(fv_it.handle());
	//			// normalize each point
	//
	//			faceNormals.push_back(p[0]);
	//			faceNormals.push_back(p[1]);
	//			faceNormals.push_back(p[2]);
	//			faceNormals.push_back(1.0f);
	//		}
	//	}
	//
	//	_mesh.release_vertex_normals();
	//	_mesh.release_face_normals();
	//
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, faceNormals.size() * sizeof(float), faceNormals.data());

}

void Model::init(const char* meshFile)
{
	programManager::sharedInstance()
	.createProgram("RgbShader",
			SHADERS_DIR "RgbShader.vert",
			SHADERS_DIR "RgbShader.frag");

	programManager::sharedInstance()
	.createProgram("PhongShader",
			SHADERS_DIR "PhongShader.vert",
			SHADERS_DIR "PhongShader.frag");

	programManager::sharedInstance()
	.createProgram("GuShader",
			SHADERS_DIR "GuShader.vert",
			SHADERS_DIR "GuShader.frag");


	programManager::sharedInstance()
	.createProgram("circle",
			SHADERS_DIR "CircleShader.vert",
			SHADERS_DIR "CircleShader.frag");

	_programRgb = programManager::sharedInstance().programWithID("RgbShader");
	_programGu = programManager::sharedInstance().programWithID("GuShader");
	_programPhong = programManager::sharedInstance().programWithID("PhongShader");
	_programCircle = programManager::sharedInstance().programWithID("circle");

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
		_posAttribCircle = glGetAttribLocation(_programCircle, "position");
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

		// Create and bind the object's vertex buffer:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		// Create and bind the object's element buffer:
		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

		//generate vertices to create the circle

		loadMesh(meshFile);
		computeCenterAndBoundingBox();
		genModelVertices();
		computeFaceNormals();

		setShadingMode(Model::SHADING_RGB);

		glBindVertexArray(0);

		resetMatrices();
	}


}


void Model::bindAttributes(GLuint program)
{
	// Obtain uniform variable handles:
	_modelViewUV = glGetUniformLocation(program, "modelView");
	_projectionUV = glGetUniformLocation(program, "projection");

	// Obtain attribute handles:
	_posAttrib = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(_posAttrib);
	glVertexAttribPointer(_posAttrib, // attribute handle
			4,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE,
			sizeof(glm::vec4) * 2,
			0);

	_normalAttrib = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(_normalAttrib);
	glVertexAttribPointer(_normalAttrib, // attribute handle
			4,          // number of scalars per vertex
			GL_FLOAT,   // scalar type
			GL_FALSE,
			sizeof(glm::vec4) * 2,
			(GLvoid*)(sizeof(glm::vec4)));
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
	glUseProgram(_program);

	glPolygonMode(GL_FRONT_AND_BACK, _glPolygonMode);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);

	glm::mat4 modelView  = _translateMat * _viewMat  * _rotationMat * _modelMat * _scaleMat;

	glUniformMatrix4fv(_projectionUV, 1, GL_FALSE, glm::value_ptr(_projectionMat));
	glUniformMatrix4fv(_modelViewUV, 1, GL_FALSE, glm::value_ptr(modelView));


	glDrawElements(GL_TRIANGLES, _mesh.n_faces() * 3, GL_UNSIGNED_INT, NULL);

	// Unbind the Vertex Array object
	glBindVertexArray(0);

	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::draw2D()
{
	glUseProgram(_programCircle);

	glBindVertexArray(_vaoCircle);
	glDrawArrays(GL_LINE_LOOP, 0, _numCircleVertices);

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

void Model::toggleNormalMode()
{
	if (_normalMode == NORMAL_FACE)
	{
		_normalMode = NORMAL_VERTEX;
	}
	else
	{
		_normalMode = NORMAL_FACE;
	}
}

void Model::setShadingMode(Model::shadingMode mode)
{
	_shadingMode = mode;

	switch(_shadingMode)
	{
	case SHADING_RGB:
		_program = _programRgb;
		break;
	case SHADING_GOURAUD:
		_program = _programGu;
		break;
	case SHADING_PHONG:
		_program = _programPhong;
		break;
	default:
		break;

	}
	bindAttributes(_program);

}
