/*
 * object.cpp
 */

#include "object.h"

Object::Object() : _diffuseTexture(NULL)
{
}

void Object::set_texture_map(BImage* image)
{
	_diffuseTexture = image;
}

Color3d& Object::diffuse()
{
	return _diffuse;
}

Color3d& Object::specular()
{
	return _specular;
}

Color3d& Object::ambient()
{
	return _ambient;
}

Color3d& Object::reflection()
{
	return _reflection;
}

Color3d& Object::transparency()
{
	return _transparency;
}

double& Object::index()
{
	return _index;		
}

double& Object::shining()
{
	return _shining;
}

const Color3d& Object::getDiffuse() const
{
	return _diffuse;
}

const Color3d& Object::getSpecular() const
{
	return _specular;
}

const Color3d& Object::getAmbient() const
{
	return _ambient;
}

const Color3d& Object::getReflection() const
{
	return _reflection;
}

const Color3d& Object::getTransparency() const
{
	return _transparency;
}

const double Object::getIndex() const
{
	return _index;
}

const double Object::getShining() const
{
	return _shining;
}





