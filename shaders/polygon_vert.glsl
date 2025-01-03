#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 view_proj_model;

void main()
{
    gl_Position = view_proj_model * vec4(aPos, 0.0, 1.0);
    ourColor = aColor;
}