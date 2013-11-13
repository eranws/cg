#version 330

uniform vec4 fillColor;
out vec4 outColor;
vec4 whiteFill = vec4(1.0, 1.0, 1.0, 1.0);



void main()
{
	//vec2 lightCenter1 = normalize(lightSource1 - ballCenter) * (radius / 2) + ballCenter;
	//vec2 lightCenter2 = normalize(lightSource2 - ballCenter) * (radius / 2) + ballCenter;	
	
	//float distFromCenter1  = distance(lightCenter1, gl_FragCoord.xy);
	//float distFromCenter2  = distance(lightCenter2, gl_FragCoord.xy);
	//float intensity1 = min(distFromCenter1, distFromCenter2) / (radius / 3);
	outColor = whiteFill;
	
}

	