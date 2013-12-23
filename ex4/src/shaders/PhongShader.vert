#version 330

#define MY_PI 3.1415926

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec3 viewNormal;
out vec3 viewPosition;
out vec3 realPosition;

out vec2 fragTexCoord;

uniform mat4 modelView;
uniform mat4 projection;

void main() {
	
	gl_Position = projection * modelView * position;
	
	viewNormal = (modelView * normal).xyz;
	viewPosition = (modelView * position).xyz;
	
	realPosition = position.xyz;

	// Set texture coordinates using spherical mapping:
	{
		vec3 pos = (reflect(gl_Position.xyz, normalize(viewNormal.xyz)));

		float theta = atan(pos.x/pos.z);
		float phi   = atan(pos.y/length(vec2(pos.x,pos.z)));
		float r     = length(pos.xyz);

		float u = (theta + MY_PI) / (2 * MY_PI);
		float v = (phi + MY_PI/2)  / MY_PI;
		float a = 1;
		float b = 1;
		fragTexCoord.x = 1.0 - (a*u - floor(a * u));
		fragTexCoord.y = 1.0 - (b*v - floor(b * v));
	}
    	 
}
