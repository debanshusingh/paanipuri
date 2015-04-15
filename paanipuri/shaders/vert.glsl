#version 410

in vec3 vs_Position;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
    //built-in things to pass down the pipeline
    gl_Position = u_projection * u_view * u_model * vec4(vs_Position, 1.0);
}
