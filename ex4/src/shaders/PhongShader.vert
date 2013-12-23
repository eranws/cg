#version 330


layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec3 viewNormal;
out vec3 viewPosition;
out vec3 realPosition;
out vec3 position3;

uniform mat4 modelView;
uniform mat4 projection;

void main() {
	
	gl_Position = projection * modelView * position;
	position3 = gl_Position.xyz;

	viewNormal = (modelView * normal).xyz;
	viewPosition = (modelView * position).xyz;
	realPosition = position.xyz;

}
