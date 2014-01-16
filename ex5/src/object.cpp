/*
* object.cpp
*/

#include "object.h"

Object::Object() :
	_ambient(COLOR_DEFAULT_AMBIENT),
	_diffuse(COLOR_DEFAULT),
	_specular(COLOR_DEFAULT),
	_reflection(COLOR_DEFAULT),
	_transparency(COLOR_DEFAULT),
	_shining(0.0),
	_index(1.0),
	_diffuseTexture(NULL),
	_parent(NULL)
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

	return _parent ? _parent->getDiffuse() : _diffuse;
}

const Color3d& Object::getSpecular() const
{
	return _parent ? _parent->getSpecular() : _specular;
}

const Color3d& Object::getAmbient() const
{
	return _parent ? _parent->getAmbient() :_ambient;
}

const Color3d& Object::getReflection() const
{
	return _parent ? _parent->getReflection() : _reflection;
}

const Color3d& Object::getTransparency() const
{
	return _parent ? _parent->getTransparency() : _transparency;
}

const double Object::getIndex() const
{
	return _parent ? _parent->getIndex() : _index;
}

const double Object::getShining() const
{
	return _parent ? _parent->getShining() : _shining;
}

const BImage* Object::getDiffuseTexture() const
{
	return _parent ? _parent->getDiffuseTexture() : _diffuseTexture;
}

bool Object::hasTexture() const
{
	return  _parent ? _parent->hasTexture() : _diffuseTexture != NULL;
}

bool Object::isRefractive() const
{
	return _parent ? _parent->isRefractive() : _index > 1.0;
}
