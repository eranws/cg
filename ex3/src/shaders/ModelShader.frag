#version 330

uniform vec4 fillColor;
out vec4 outColor;
vec4 whiteFill = vec4(1.0, 1.0, 1.0, 1.0);
in vec4 color;



void main()
{
	outColor =  color;
}

	