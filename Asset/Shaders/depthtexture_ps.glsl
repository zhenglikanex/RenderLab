#version 450

in vec3 V;
in vec2 UV;
out vec3 color;

uniform sampler2DArray depthSampler;
uniform float layer_index;

void main()
{
    color = V;
}