#version 450

in vec3 fragPos;
in vec3 fragNormal;
in vec2 UV;
uniform vec4 viewPosition;
uniform samplerCube skybox;
uniform sampler2D normalMap;
uniform sampler2D diffuseMap;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform float dt;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{

	vec3 waterColor = texture(diffuseMap,vec2(0.65,1)).rgb;
	// 从法线贴图范围[0,1]获取法线
    vec3 N1 = texture(normalMap, UV + dt * vec2(0.05f,0.03f)).rgb;
	vec3 N2 = texture(normalMap, UV + dt * vec2(-0.015f,-0.07f)).rgb;
    // 将法线向量转换为范围[-1,1]
    vec3 N = normalize((N1 + N2)/2);   
	N = mat3(transpose(inverse(modelMatrix))) * N;
	//vec3 N = normalize(fragNormal);
	vec3 I = normalize(fragPos - vec3(viewPosition));
	vec3 V = normalize(vec3(viewPosition) - fragPos);
	vec3 R = reflect(I,N);
	vec3 Color = vec3(texture(skybox,R));

	//float ratio = 1.0f / 1.33f;
	//vec3 I = normalize(fragPos - vec3(viewPosition));
	//vec3 R = refract(I,normalize(fragNormal),ratio);
	//gl_FragColor = vec4(texture(skybox,R).rgb,1.0f);

	vec3 F0 = vec3(0.15f,0.15f,0.15f);
	vec3 kS = fresnelSchlick(dot(V,N),F0);
	vec3 kD = vec3(1.0f) - kS;
	
	//vec3 waterColor = vec3(0.058f,)

	gl_FragColor = vec4(Color.rgb * kS + waterColor * kD,1.0f); //vec4(Color * F0,1.0f);
	//gl_FragColor = vec4(N,1.0f);
}