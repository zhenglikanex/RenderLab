#version 450

in vec3 texCoord;

uniform samplerCube skybox;

void main()
{
	gl_FragColor = texture(skybox,texCoord);
	
}