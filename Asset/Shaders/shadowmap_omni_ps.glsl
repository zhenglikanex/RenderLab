in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

// 自己计算深度，计算时用世界坐标系下的坐标比较
void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    
    lightDistance = lightDistance / far_plane;

    gl_FragDepth = lightDistance;
}

// 什么都不做，让OpenGL配置深度贴图的深度值,计算阴影时得把当前点转换到光照的空间
//void main()
//{
//    Color = vec4(0.0f);
//}