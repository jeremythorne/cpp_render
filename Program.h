#include <GLES3/gl3.h>
#include <string>
#include <vector>

struct Program {
    bool compile(const char *vert, const char* frag, 
        std::vector<std::string> uniforms,
        std::vector<std::string> attributes
        );
    void use();

    GLuint program;
};
