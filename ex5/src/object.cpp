/*
 * object.cpp
 */

#include "object.h"

Object::Object() : _diffuseTexture(NULL)
{

}

void Object::set_texture_map(BImage* image)
{

}

Color3d& Object::diffuse()
{
	return Color3d(); //TODO
}

Color3d& Object::specular()
{
	return Color3d(); //TODO
}

Color3d& Object::ambient()
{
	return Color3d(); //TODO
}

Color3d& Object::reflection()
{
	return Color3d(); //TODO
}

Color3d& Object::transparency()
{
	return Color3d(); //TODO
}

double& Object::index()
{
	double v = 0.0; //TODO
	return v;		
}

double& Object::shining()
{
	double v = 0.0; //TODO
	return v;		
}

const Color3d& Object::getDiffuse() const
{
	return Color3d(); //TODO
}

const Color3d& Object::getSpecular() const
{
	return Color3d(); //TODO
}

const Color3d& Object::getAmbient() const
{
	return COLOR_RED; //TODO
}

const Color3d& Object::getReflection() const
{
	return Color3d(); //TODO
}

const Color3d& Object::getTransparency() const
{
	return Color3d(); //TODO
}

const double Object::getIndex() const
{
	return 0;
}

const double Object::getShining() const
{
	return 0;
}





