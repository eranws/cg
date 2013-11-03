/*
 * Ball.h
 *
 *  Created on: Oct 24, 2013
 *      Author: ilansh
 */

#ifndef BALL_H_
#define BALL_H_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>

#define MIN_COLOR 0.7

class Model;

struct Ball {

	Ball(int xPos, int yPos, const Model* model);
	void update();

	//The color of the ball
	glm::vec3 _color;

	//The radius of the ball
	GLfloat _radius;
	GLfloat _initialRadius;
	void setRadius(float radius);

	static const GLfloat DEFAULT_RAD = 30;


	//The position of the ball
	glm::vec2 _pos;

	//Moving direction of the ball (velocity)
	glm::vec2 _dir;

	//The model
	const Model* _model;
};

#endif /* BALL_H_ */
