#version 330 core

in vec3 outColor;

out vec4 OutColor;

void main()
{
    OutColor = vec4(outColor,1.0);
}
