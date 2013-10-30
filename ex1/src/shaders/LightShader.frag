#version 330

uniform vec4 fillColor;

out vec4 outColor;

uniform vec2 ballCenter;

uniform float radius;

uniform vec2 lightSource;

vec4 whiteFill = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	vec2 lightCenter = normalize(lightSource - ballCenter) * (radius / 2) + ballCenter;
	float distFromCenter  = distance(lightCenter, gl_FragCoord.xy);
	float intensity = distFromCenter / radius;
	outColor = mix(whiteFill, fillColor, intensity);
}

	