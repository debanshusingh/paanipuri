#version 410

in vec3 vs_Position;
in vec3 vs_Color;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 outColor;

void main()
{
    //built-in things to pass down the pipeline
    gl_Position = u_projection * u_view * u_model * vec4(vs_Position, 1.0);
    gl_PointSize = 1.0f;
    outColor = vs_Color;
}
