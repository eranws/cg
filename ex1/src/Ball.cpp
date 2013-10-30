/*
 * Ball.cpp
 *
 *  Created on: Oct 24, 2013
 *      Author: ilansh
 */


#include "ShaderIO.h"
#include "Model.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/gtx/random.hpp>


#include <stdlib.h>
#include "Ball.h"
#include "Model.h"

// returns random number between 0 <= x < 1
float randf()
{
	return ((float) rand() / RAND_MAX);
}

Ball::Ball(int xPos, int yPos, const Model* model)
{
	_pos.x = xPos;
	_pos.y = yPos;

	_model = model;

	_color = glm::vec3(randf(), randf(), randf());
	_dir = glm::normalize(glm::vec2(randf()*2 - 1, randf()*2 - 1));

	_initialRadius = DEFAULT_RAD;

	// checking boundary conditions
	_initialRadius = std::min(_initialRadius, _pos.x);
	_initialRadius = std::min(_initialRadius, _pos.y);
	_initialRadius = std::min(_initialRadius, _model->getWidth() - _pos.x);
	_initialRadius = std::min(_initialRadius, _model->getHeight() - _pos.y);

	// check with other balls
	for (size_t j=0; j<_model->_balls.size();j++)
	{
		float dist = glm::distance(_pos, _model->_balls[j]._pos);
		_initialRadius = std::min(_initialRadius, dist);
	}


	_radius = _initialRadius;

}


void Ball::update()
{
	if(_pos.x + _radius >= _model->getWidth() || _pos.x - _radius <= 0) {
		_dir.x *= -1;
	}
	if(_pos.y + _radius >= _model->getHeight() || _pos.y - _radius <= 0) {
		_dir.y *= -1;
	}

	_pos += glm::vec2(_dir.x * 3, _dir.y * 3);
//	_pos += _dir;
}


void Ball::setRadius(float r)
{
	// check boundaries so we won't expand ball out of the window frame
	// we add also dir to avoid jittering caused from resize and translation
	r = std::min(r, _model->getWidth() - _pos.x + _dir.x);
	r = std::min(r, _pos.x - _dir.x);
	r = std::min(r, _model->getHeight() - _pos.y + _dir.y);
	r = std::min(r, _pos.y - _dir.y);

	_radius = r;
}
