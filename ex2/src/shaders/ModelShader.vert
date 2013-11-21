#version 330

layout(location = 0) in vec4 position;
out vec4 color;

uniform mat4 transform;
uniform vec3 lowerLeft;
uniform vec3 upperRight;

void main() {

	
	color = (position * 3 + 1) / 2;
//	color = (position + 1) / 2;
//	color = (position + (-1) * vec4(lowerLeft, 0.0)) / (vec4(upperRight, 1.0) - vec4(lowerLeft, 1.0));
    gl_Position = transform * position;
   	 
}
