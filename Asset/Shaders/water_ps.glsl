#version 450

in vec3 fragPos;
in vec3 fragNormal;

uniform vec4 viewPosition;
uniform samplerCube skybox;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	
	vec3 N = normalize(fragNormal);
	vec3 I = normalize(fragPos - vec3(viewPosition));
	vec3 V = normalize(vec3(viewPosition) - fragPos);
	vec3 R = reflect(I,N);
	vec3 Color = vec3(texture(skybox,R));

	//float ratio = 1.0f / 1.33f;
	//vec3 I = normalize(fragPos - vec3(viewPosition));
	//vec3 R = refract(I,normalize(fragNormal),ratio);
	//gl_FragColor = vec4(texture(skybox,R).rgb,1.0f);

	vec3 F0 = vec3(0.15,0.15f,0.15);
	vec3 kS = fresnelSchlick(dot(V,N),F0);
	vec3 kD = vec3(1.0f) - kS;
	
	vec3 waterColor = vec3(0.058f,)

	gl_FragColor = vec4(Color.rgb * kS,1.0f); //vec4(Color * F0,1.0f);
}