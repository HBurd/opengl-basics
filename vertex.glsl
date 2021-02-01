#version 330

layout (location = 0) in vec2 offset;
//vec3 (location = 1) normal;

void main()
{
    gl_Position = vec4(offset, 0.0f, 1.0f);
}
