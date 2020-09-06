#version 450

in vec3 fragPos;
in vec3 fragNormal;

uniform vec3 viewPosition;
uniform samplerCube skybox;

void main()
{
	mat3 matrix = mat3(
		1.0f,0.0f,0.0f,
		0.0f,0.0f,-1.0f,
		0.0f,1.0f,0.0f
	);
	vec3 normal = matrix * fragNormal;
	vec3 I = normalize(fragPos - viewPosition);
	vec3 R = reflect(I,normalize(fragNormal));
	R =  matrix * R;
	gl_FragColor = texture(skybox,R);
	//gl_FragColor = vec4(0.0f,0.0f,0.0f,1.0f);
}