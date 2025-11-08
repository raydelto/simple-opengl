#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class OpenGLInit
{
public:
    OpenGLInit();
    ~OpenGLInit();
    static void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode);
    static void glfw_onFramebufferSize(GLFWwindow *window, int width, int height);
    void showFPS();
    bool initOpenGL();
    GLFWwindow *GetWindow() { return _window; }
    static bool _pause;
    static bool _wireframe;
    static int _windowWidth;
    static int _windowHeight;

private:
    GLFWwindow *_window;
    double _elapsedSeconds;
    static constexpr const char *APP_TITLE = "MD2 Loader by Raydelto Hernandez v1.0";
};
