#version 330

layout(location = 0) in vec4 position;

out vec2 pos;

void main()
{
	pos.x = position.x;
	pos.y = position.y;

    gl_Position = position;
}
