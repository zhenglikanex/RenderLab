#version 450
layout(location = 0) in vec3 inputPsition;
layout(location = 1) in vec2 inputUV;

out vec2 UV;
out vec3 V;
void main()
{
    V = inputPsition;
    gl_Position = vec4(inputPsition,1.0f);
    UV = inputUV;
}