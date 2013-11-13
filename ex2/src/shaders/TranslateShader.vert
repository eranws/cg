#version 330

layout(location = 0) in vec4 position;
out vec4 color;

uniform mat4 transform;

void main() {
	color = position;//vec4(1.0, 0.0, 0.0, 1.0);
    gl_Position = transform * position;
}
