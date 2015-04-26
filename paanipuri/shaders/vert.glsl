#version 330 core

in vec3 vs_Position;
in vec3 vs_Color;
in vec3 vs_Size;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 outColor;
out vec2 outTexCoords;

void main()
{
    //built-in things to pass down the pipeline
    gl_Position = u_projection * u_view * u_model * vec4(vs_Position, 1.0);
    gl_PointSize = vs_Size.x;
    outColor = vs_Color;
}
