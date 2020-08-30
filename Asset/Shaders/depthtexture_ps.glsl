#version 450

in vec2 UV;
out vec3 color;

uniform sampler2DArray depthSampler;
uniform float layer_index;

void main()
{
    color = pow(texture(depthSampler,vec3(UV,0)).rrr, vec3(1.0f));
}