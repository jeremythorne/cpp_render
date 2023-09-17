#include "Window.h"
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

bool Window::init() {
    if (!glfwInit())
    {
        return false;
    }
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window) {
        return false;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return true;
}

Window::~Window() {
    cleanup();
}

void Window::cleanup() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

bool Window::loop(std::function<void(float, int, int)> fn) {
    if (!window) {
        return false;
    }
    if (glfwWindowShouldClose(window)) {
        return false;
    }
    double time = glfwGetTime();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    fn(time, width, height);

    glfwSwapBuffers(window);
    glfwPollEvents();
    return true;
}
