#version 450

layout(location = 0) in vec3 inputPosition;

// update per frame
uniform mat4 depthVP;

uniform mat4 modelMatrix;

void main()
{
    gl_Position = depthVP * modelMatrix * vec4(inputPosition,1.0f);
}
