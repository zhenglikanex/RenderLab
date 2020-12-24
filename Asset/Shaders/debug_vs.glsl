in vec3 inputPosition;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(inputPosition,1.0);
}