#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec3 viewNormal;
out vec3 viewPosition;

uniform mat4 modelView;
uniform mat4 projection;

void main() {
	
	gl_Position = projection * modelView * position;
	
	viewNormal = (modelView * normal).xyz;
	viewPosition = (modelView * position).xyz;
	
    	 
}
