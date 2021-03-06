#version 330
#define SQUARE_FACTOR 50 //higher number means smaller squares

uniform vec4 fillColor;
uniform vec4 fillBlack = vec4(0.0, 0.0, 0.0, 0.0);

out vec4 outColor;
in vec2 pos;

void main()
{
	bool fillX = int((1+pos.x) * SQUARE_FACTOR) % 2 == 0; // add one to avoid bad result when performing modulu on negatives
	bool fillY = int((1+pos.y) * SQUARE_FACTOR) % 2 == 0;

	bool toFill = fillX ^^ fillY; //XOR
	outColor = toFill ? fillColor : fillBlack;
}
