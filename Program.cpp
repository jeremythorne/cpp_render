#include <GLES3/gl3.h>

#include "Program.h"

    GLint mvp_location, vpos_location, vcol_location;

bool check_shader(GLint shader, const char* name) {
    char buf[255];
    int status = 0;
    int len = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        glGetShaderInfoLog(shader, sizeof(buf), &len, buf);
        printf("failed to compile %s shader\n%s\n", name, buf);
        return false;
    }
    return true;
}

bool check_program(GLint program) {
    char buf[255];
    int status = 0;
    int len = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status != GL_TRUE) {
        glGetProgramInfoLog(program, sizeof(buf), &len, buf);
        printf("failed to link program\n%s\n", buf);
        return false;
    }
}

bool Program::compile(const char *vert, const char* frag, 
        std::vector<std::string> uniforms,
        std::vector<std::string> attributes) {
    GLuint vertex_shader, fragment_shader; 
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert, NULL);
    glCompileShader(vertex_shader);
 
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag, NULL);
    glCompileShader(fragment_shader);
 
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    if (!check_shader(vertex_shader, "vertex")) {
        return false;
    }
    if (!check_shader(fragment_shader, "vertex")) {
        return false;
    }
    if (!check_program(program)) {
        return false;
    }

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
 
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          0, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          0, (void*) (sizeof(float) * 2));
 
}


void Program::use() {
    glUseProgram(program);
}