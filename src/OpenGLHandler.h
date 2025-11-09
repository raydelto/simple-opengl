#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class OpenGLHandler
{
public:
    OpenGLHandler();
    ~OpenGLHandler();
    static void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode);
    static void glfw_onFramebufferSize(GLFWwindow *window, int width, int height);
    void showFPS();
    bool init();
    GLFWwindow* getWindow() const { return _window; }
    
    // Getters for state
    static bool isPaused() { return _pause; }
    static bool isWireframe() { return _wireframe; }
    static int getWindowWidth() { return _windowWidth; }
    static int getWindowHeight() { return _windowHeight; }

private:
    GLFWwindow *_window;
    double _elapsedSeconds;
    static constexpr const char *APP_TITLE = "MD2 Loader by Raydelto Hernandez v1.0";
    
    // Private static state
    static bool _pause;
    static bool _wireframe;
    static int _windowWidth;
    static int _windowHeight;
};
