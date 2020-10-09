in vec3 inputPosition;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inputPosition,1.0);
}