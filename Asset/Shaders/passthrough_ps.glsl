in vec2 UV;

out vec3 color;

uniform sampler2D depthSampler;

void main()
{
    color = pow(texture(depthSampler, UV).rrr, vec3(1.0f));
}