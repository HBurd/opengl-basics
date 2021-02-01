#version 330

vec3 (location = 0) offset;
vec3 (location = 1) normal;

void main()
{
    gl_Position = offset;
}
