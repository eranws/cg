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

class Model;

struct Ball {

	Ball(int xPos, int yPos, const Model* model);
	void update();

	//The color of the ball
	glm::vec3 color;

	//The radius of the ball
	GLfloat radius;
	GLfloat initialRadius;
	void setRadius(float radius);

	static const GLfloat DEFAULT_RAD = 50;


	//The position of the ball
	glm::vec2 pos;

	//Moving direction of the ball (velocity)
	glm::vec2 dir;

	//The model
	const Model* _model;
};

#endif /* BALL_H_ */
