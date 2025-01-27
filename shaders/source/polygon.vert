#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

layout(location = 0) out vec3 ourColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model_view_proj;
} ubo;

void main()
{
    gl_Position = ubo.model_view_proj * vec4(aPos, 0.0, 1.0);
    ourColor = aColor;
}