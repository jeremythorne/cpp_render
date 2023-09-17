#include <GLFW/glfw3.h>
#include <functional>

struct Window {
    bool init();
    bool loop(std::function<void(float, int, int)> fn);
    void cleanup();

    ~Window();

    GLFWwindow* window = nullptr;
};