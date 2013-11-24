#version 330

layout(location = 0) in vec4 position;
out vec4 color;

uniform mat4 transform;
uniform vec3 lowerLeft;
uniform vec3 upperRight;

void main() {

	
	color = (position + 1) / 2;
    gl_Position = transform * position;
   	 
}
