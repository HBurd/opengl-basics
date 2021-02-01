#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

const float square[12] = {
    /* Lower triangle */
    -0.5f, -0.5f,
     0.5f, -0.5f,
    -0.5f,  0.5f,

    /* Upper triangle */
     0.5f,  0.5f,
    -0.5f,  0.5f,
     0.5f, -0.5f,
};

void build_cube(float (*points)[108], float (*normals)[108])
{
    int i = 0;

    /* front face */
    for (; i < 6; ++i)
    {
        (*points)[i * 3]     = square[i * 2];
        (*points)[i * 3 + 1] = square[i * 2 + 1];
        (*points)[i * 3 + 2] = 0.5f;

        (*normals)[i * 3]     = 0.0f;
        (*normals)[i * 3 + 1] = 0.0f;
        (*normals)[i * 3 + 2] = 1.0f;
    }

    /* back face */
    for (; i < 12; ++i)
    {
        (*points)[i * 3]     = -square[i * 2];
        (*points)[i * 3 + 1] =  square[i * 2 + 1];
        (*points)[i * 3 + 2] = -0.5f;

        (*normals)[i * 3]     =  0.0f;
        (*normals)[i * 3 + 1] =  0.0f;
        (*normals)[i * 3 + 2] = -1.0f;
    }

    /* left face */
    for (; i < 18; ++i)
    {
        (*points)[i * 3]     = -0.5f;
        (*points)[i * 3 + 1] =  square[i * 2 + 1];
        (*points)[i * 3 + 2] =  square[i * 2];

        (*normals)[i * 3]     = -1.0f;
        (*normals)[i * 3 + 1] =  0.0f;
        (*normals)[i * 3 + 2] =  0.0f;
    }

    /* right face */
    for (; i < 24; ++i)
    {
        (*points)[i * 3]     =  0.5f;
        (*points)[i * 3 + 1] =  square[i * 2 + 1];
        (*points)[i * 3 + 2] = -square[i * 2];

        (*normals)[i * 3]     = 1.0f;
        (*normals)[i * 3 + 1] = 0.0f;
        (*normals)[i * 3 + 2] = 0.0f;
    }

    /* top face */
    for (; i < 30; ++i)
    {
        (*points)[i * 3]     =  square[i * 2];
        (*points)[i * 3 + 1] =  0.5f;
        (*points)[i * 3 + 2] = -square[i * 2 + 1];

        (*normals)[i * 3]     = 0.0f;
        (*normals)[i * 3 + 1] = 1.0f;
        (*normals)[i * 3 + 2] = 0.0f;
    }

    /* bottom face */
    for (; i < 36; ++i)
    {
        (*points)[i * 3]     =  square[i * 2];
        (*points)[i * 3 + 1] = -0.5f;
        (*points)[i * 3 + 2] =  square[i * 2 + 1];

        (*normals)[i * 3]     =  0.0f;
        (*normals)[i * 3 + 1] = -1.0f;
        (*normals)[i * 3 + 2] =  0.0f;
    }
}


GLint compile_shader(const char* filename, GLuint shader_type)
{
    FILE* shader_file = fopen(filename, "r");
    if (!shader_file)
    {
        fprintf(stderr, "Unable to open shader %s.\n", filename);
        return 0;
    }
    fseek(shader_file, 0, SEEK_END);
    size_t file_len = ftell(shader_file);
    GLchar* shader_text = (GLchar*) malloc(file_len + 1); // +1 for null terminator
    rewind(shader_file);
    fread(shader_text, 1, file_len, shader_file);

    // add null terminator
    shader_text[file_len] = 0;

    GLuint shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, (const GLchar**)&shader_text, NULL);
    glCompileShader(shader);

    free(shader_text);

    GLint log_size = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
    if (log_size > 1)
    {
        GLchar* info_log = (GLchar*) malloc(log_size);
        glGetShaderInfoLog(shader, log_size, NULL, info_log);
        printf("%s info log: %s\n", filename, info_log);
        free(info_log);
    }

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        fprintf(stderr, "Shader compilation failed.\n");
    }

    return shader;
}

GLuint link_program(GLint vshader, GLint fshader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        fprintf(stderr, "Shader program linking failed.\n");
    }
    return program;
}

int main()
{
    if (!glfwInit())
    {
        printf("Could not init glfw\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL test", NULL, NULL);

    if (!window)
    {
        printf("Could not open window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit())
    {
        printf("Could not init GLEW\n");
        return -1;
    }

    /* Make vertex buffer */

    GLuint vbo;
    glGenBuffers(1, &vbo);

    // bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /* Fill vertex buffer */
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    /* Make vertex array */

    GLuint vao;
    glGenVertexArrays(1, &vao);

    // bind the vertex array
    glBindVertexArray(vao);

    /* Point vertex array to buffer */
    glEnableVertexAttribArray(0);
    
    // explain the data format
    // tightly packed pairs of floats
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint program = link_program(compile_shader("vertex.glsl", GL_VERTEX_SHADER), compile_shader("fragment.glsl", GL_FRAGMENT_SHADER));

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(square) / 2);

        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
