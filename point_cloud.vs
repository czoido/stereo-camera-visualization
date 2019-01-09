#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 color = mix(aCol,vec3(141.0/255,253.0/255,1.0), 0.5);
    vertexColor = vec4(color,1.0);
}
