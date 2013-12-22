#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec4 color;

uniform mat4 modelView;
uniform mat4 projection;

void main() {
	color = (position + 1) / 2;
    gl_Position = projection * modelView * position;
   	 
}
