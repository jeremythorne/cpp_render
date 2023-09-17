#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

struct State {
    GLFWwindow* window = nullptr;
    GLuint vertex_buffer,  program;
    GLint mvp_location, vpos_location, vcol_location;
} state;

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};
 
static const char* vertex_shader_text =
"#version 300 es\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 300 es\n"
"precision mediump float;\n"
"in vec3 color;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(color, 1.0);\n"
"}\n";

void setup() {
    if (!glfwInit())
    {
        return;
    }
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    state.window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!state.window) {
        return;
    }
    glfwSetKeyCallback(state.window, key_callback);
    glfwMakeContextCurrent(state.window);
    glfwSwapInterval(1);


    glGenBuffers(1, &state.vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, state.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint vertex_shader, fragment_shader; 
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
 
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
 
    state.program = glCreateProgram();
    glAttachShader(state.program, vertex_shader);
    glAttachShader(state.program, fragment_shader);
    glLinkProgram(state.program);

    char buf[255];
    int status = 0;
    int len = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        glGetShaderInfoLog(vertex_shader, sizeof(buf), &len, buf);
        printf("failed to compile vertex shader\n%s\n", buf);
        return;
    }
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        glGetShaderInfoLog(fragment_shader, sizeof(buf), &len, buf);
        printf("failed to compile fragment shader\n%s\n", buf);
        return;
    }

    glGetProgramiv(state.program, GL_LINK_STATUS, &status);
    if(status != GL_TRUE) {
        glGetProgramInfoLog(state.program, sizeof(buf), &len, buf);
        printf("failed to link program\n%s\n", buf);
        return;
    }

    state.mvp_location = glGetUniformLocation(state.program, "MVP");
    state.vpos_location = glGetAttribLocation(state.program, "vPos");
    state.vcol_location = glGetAttribLocation(state.program, "vCol");
 
    glEnableVertexAttribArray(state.vpos_location);
    glVertexAttribPointer(state.vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(state.vcol_location);
    glVertexAttribPointer(state.vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));
 
}

void cleanup() {
    if (state.window) {
        glfwDestroyWindow(state.window);
    }
    glfwTerminate();
}

void run() {
    if (!state.window) {
        return;
    }
    while (!glfwWindowShouldClose(state.window)) {
        // Keep running
        double time = glfwGetTime();
        int width, height;
        float ratio;
        glm::mat4 m, p, mvp;
 
        glfwGetFramebufferSize(state.window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
 
        m = glm::rotate(glm::mat4(1.0f), (float) time, {0.0f, 0.0f, 1.0f});
        p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mvp = p * m;
 
        glUseProgram(state.program);
        glUniformMatrix4fv(state.mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
        glDrawArrays(GL_TRIANGLES, 0, 3);
 
        glfwSwapBuffers(state.window);
        glfwPollEvents();
    }
}

int main() {
    setup();
    run();
    cleanup();
    return 0;
}