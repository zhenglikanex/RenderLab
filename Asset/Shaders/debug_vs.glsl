#version 150

in vec3 inputPosition;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inputPosition,1.0);
}