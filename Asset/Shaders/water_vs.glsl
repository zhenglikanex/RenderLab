#version 450

layout(location = 0) in vec3 inputPosition;

out vec3 fragPos;
out vec3 fragNormal;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
// update per draw call
uniform mat4 modelMatrix;

// update per frame
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float dt;

const float pi = 3.1415926f;

vec3 CaculGerstnerWave(vec4 vertex,float steepness,float amplitude,vec2 dir,float frequency,float phase)
{
	float vertexDotDir =  dot(dir,vertex.xz);
	dir = normalize(dir);
	float x =  steepness * amplitude * dir.x * cos(frequency * vertexDotDir + phase * dt);
	float z =  steepness * amplitude * dir.y * cos(frequency * vertexDotDir + phase * dt);
	float y =  amplitude * sin(frequency * vertexDotDir + phase * dt);
	return vec3(x,y,z);
}

vec3 CaculNoraml(vec4 vertex,float steepness,float amplitude,vec2 dir,float frequency,float phase)
{
	dir = normalize(dir);
	float x = dir.x * amplitude * frequency * cos(dot(frequency * dir,vertex.xz) + phase * dt);
	float z = dir.y * amplitude * frequency * cos(dot(frequency * dir,vertex.xz) + phase * dt);
	float y = steepness * amplitude * frequency * sin(dot(frequency * dir,vertex.xz) + phase * dt);

	return vec3(x,y,z);
}
//vec2 dir[5] = {  }

void main()
{
	vec4 worldPos =  worldMatrix * modelMatrix * vec4(inputPosition,1.0f);

	/*
	vec3 pos = vec3(worldPos);
	pos += CaculGerstnerWave(worldPos,1.5f,1.0f,vec2(1.0f,0.0f),0.2f,1.0f);
	pos += CaculGerstnerWave(worldPos,1.0f,1.0f,vec2(1.0f,1.0f),0.1f,1.0f);
	pos += CaculGerstnerWave(worldPos,0.2f,1.0f,vec2(0.5f,1.0f),1.0f,1.5f);
	//worldPos.xz += pos.xz;
	//worldPos.y = pos.y;
	*/
	vec3 normal = vec3(0.0f,0.0f,0.0f);
	normal += CaculNoraml(worldPos,1.5f,1.0f,vec2(1.0f,0.0f),0.2f,1.0f);
	normal += CaculNoraml(worldPos,1.0f,1.0f,vec2(1.0f,1.0f),0.1f,1.0f);
	normal += CaculNoraml(worldPos,0.2f,1.0f,vec2(0.5f,1.0f),1.0f,1.5f);
	normal = vec3(0.0f,1.0f,0.0f) - normal;

	fragNormal = vec3(0.0f,0.0f,1.0f);
	fragPos = vec3(worldPos);
	//float filterAmp = 3.0f * 3.1415926f / 3.1415926f / 2.0f - 1.0f;
	//filterAmp = clamp(filterAmp,0.0f,1.0f);
	//worldPos.z = sin(0.1f * dot(dir[0],worldPos.xy) + dt * 2.0f);
	//worldPos.z = worldPos.z + sin( 0.3f * dot(dir[1],worldPos.xy) + dt * 2.0f );
	
	gl_Position = projectionMatrix * viewMatrix * worldPos;
}