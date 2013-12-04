#version 330


uniform float specExp = 200.0;
	
uniform vec3 light1 = vec3(3.0, 2.0, 1.0);
uniform vec3 light2 = vec3(-3.0, 0.0, 1.0);


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
	

	//Ambient
	vec3 ambient = ka * ambientColor;


	vec3 lightDir1 = normalize(viewPosition - light1);
	vec3 lightDir2 = normalize(viewPosition - light2);
	vec3 n = normalize(viewNormal);

	//Diffuse
	vec3 diffuse1 = lightColor1 * kd * max(0.0, dot(n, lightDir1));
	vec3 diffuse2 = lightColor2 * kd * max(0.0, dot(n, lightDir2));
	
	//Specular
	vec3 eye = normalize(-viewPosition);
	
	vec3 s1 = normalize(reflect(lightDir1, n));
	vec3 spec1 = ks * specularColor * pow(max(dot(eye, s1), 0.0), specExp);

	vec3 s2 = normalize(reflect(lightDir2, n));
	vec3 spec2 = ks * specularColor * pow(max(dot(eye, s2), 0.0), specExp);


	vec3 shade;
	shade += ambient;
	shade += diffuse1 + diffuse2;
	shade += spec1 + spec2;
	
	outColor =  vec4(shade, 1.0);
	
}