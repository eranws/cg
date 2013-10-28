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
	pos.x = xPos;
	pos.y = yPos;

	_model = model;

	color = glm::vec3(randf(), randf(), randf());
	dir = glm::normalize(glm::vec2(randf()*2 - 1, randf()*2 - 1));

	initialRadius = DEFAULT_RAD;

	// checking boundary conditions
	initialRadius = std::min(initialRadius, pos.x);
	initialRadius = std::min(initialRadius, pos.y);
	initialRadius = std::min(initialRadius, _model->getWidth() - pos.x);
	initialRadius = std::min(initialRadius, _model->getHeight() - pos.y);

	// check with other balls
	for (size_t j=0; j<_model->_balls.size();j++)
	{
		float dist = glm::distance(pos, _model->_balls[j].pos);
		initialRadius = std::min(initialRadius, dist);
	}



	radius = initialRadius;
}


void Ball::update()
{
	if(pos.x + radius >= _model->getWidth() || pos.x - radius <= 0) {
		dir.x *= -1;
	}
	if(pos.y + radius >= _model->getHeight() || pos.y - radius <= 0) {
		dir.y *= -1;
	}

	pos += dir;
}


void Ball::setRadius(float r)
{
	// check boundaries so we won't expand ball out of the window frame
	// we add also dir to avoid jittering caused from resize and translation
	r = std::min(r, _model->getWidth() - pos.x + dir.x);
	r = std::min(r, pos.x - dir.x);
	r = std::min(r, _model->getHeight() - pos.y + dir.y);
	r = std::min(r, pos.y - dir.y);

	radius = r;
}
