#version 330


uniform float specExp = 200.0;
	
uniform vec3 light1 = vec3(3.0, 2.0, 1.0);
uniform vec3 light2 = vec3(-3.0, 0.0, 1.0);

//uniform vec3 light1 = vec3(1.0, 0.8, 1.0);
//uniform vec3 light2 = vec3(-1.0, -0.7, 1.0);

uniform vec3 ambientColor = vec3(1.0, 1.0, 1.0);

uniform vec3 lightColor1 = vec3(1.0, 0.9, 0.7); // Light 1 color
uniform vec3 lightColor2 = vec3(0.6, 0.6, 1.0); // Light 2 color
uniform vec3 specularColor = vec3(1.0, 1.0, 1.0); // Specular color

uniform vec3 ka = vec3(0.1, 0.1, 0.1); // Ambient color
uniform vec3 kd = vec3(0.3, 0.3, 0.3); // Diffuse coefficient
uniform vec3 ks = vec3(0.3, 0.3, 0.3); // Specularity coefficient

out vec4 outColor;

in vec3 viewNormal;
in vec3 viewPosition;


void main()
{
	

	vec3 eye = vec3(0,0,-3);

	//Ambient
	vec3 ambient = ka * ambientColor;

	vec3 l1 = normalize(light1 - viewPosition);
	vec3 l2 = normalize(light2 - viewPosition);
	vec3 n = normalize(viewNormal);

	float dist1 = distance(viewPosition.xyz, light1);
	float dist2 = distance(viewPosition.xyz, light2);

	//Diffuse
	vec3 diffuse1 = lightColor1 * kd * max(0.0, dot(l1, n));
	vec3 diffuse2 = lightColor2 * kd * max(0.0, dot(l2, n));
	
	//Specular
	vec3 v = normalize(eye - viewPosition);
	
	vec3 r1 = normalize(reflect(l1, n));
	vec3 spec1 = ks * specularColor * pow(max(dot(v, r1), 0.0001), specExp);

	vec3 r2 = normalize(reflect(l2, n));
	vec3 spec2 = ks * specularColor * pow(max(dot(v, r2), 0.0001), specExp);


	vec3 shade;
	shade += ambient;
	shade += (diffuse1 + diffuse2);
	shade += (spec1 + spec2);
	
	outColor =  vec4(shade, 1.0);
	
	
}