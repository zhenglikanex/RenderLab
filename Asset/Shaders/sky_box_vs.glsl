#version 450

layout(location = 0) in vec3 inputPosition;
out vec3 texCoord;

uniform mat4 modelMatrix;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	texCoord = inputPosition;
	vec4 pos = projectionMatrix * viewMatrix * worldMatrix * modelMatrix * vec4(inputPosition,1.0f);
	gl_Position = pos.xyww;
	
}