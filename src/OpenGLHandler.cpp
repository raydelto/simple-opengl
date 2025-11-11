#include "OpenGLHandler.h"
#include <iostream>
#include <sstream>

bool OpenGLHandler::_pause = false;
bool OpenGLHandler::_wireframe = false;

int OpenGLHandler::_windowWidth = 1024;
int OpenGLHandler::_windowHeight = 768;

OpenGLHandler::OpenGLHandler() : _window(nullptr)
{
}

OpenGLHandler::~OpenGLHandler()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

// Initialize GLFW and OpenGL
bool OpenGLHandler::init()
{
    // Intialize GLFW
    // GLFW is configured.  Must be called before calling any GLFW functions
    if (!glfwInit())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create an OpenGL 3.3 core context window
    _window = glfwCreateWindow(_windowWidth, _windowHeight, APP_TITLE, NULL, NULL);
    if (_window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(_window);

    // Initialize GLEW (The OpenGL Extension Wrangler)
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Set the required callback functions
    glfwSetKeyCallback(_window, glfw_onKey);
    glfwSetFramebufferSizeCallback(_window, glfw_onFramebufferSize);

    // Brown background
    glClearColor(0.25f, 0.2f, 0.15f, 1.0f);

    // Define the viewport dimensions
    glViewport(0, 0, _windowWidth, _windowHeight);
    glEnable(GL_DEPTH_TEST);

    return true;
}

// Is called whenever a key is pressed/released via GLFW
void OpenGLHandler::glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (action != GLFW_PRESS)
    {
        return;
    }

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;

    case GLFW_KEY_SPACE:
        _pause = !_pause;
        break;

    case GLFW_KEY_F1:
        _wireframe = !_wireframe;
        if (_wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        break;
    }
}

// Is called when the window is resized
void OpenGLHandler::glfw_onFramebufferSize(GLFWwindow *window, int width, int height)
{
    _windowWidth = width;
    _windowHeight = height;
    glViewport(0, 0, _windowWidth, _windowHeight);
}

// Show Frames Per Seconds
void OpenGLHandler::showFPS()
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double

    _elapsedSeconds = currentSeconds - previousSeconds;

    // Limit text updates to 4 times per second
    if (_elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / _elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        // The C++ way of setting the window title
        std::ostringstream outs;
        outs.precision(3); // decimal places
        outs << std::fixed
             << APP_TITLE << "    "
             << "FPS: " << fps << "    "
             << "Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(_window, outs.str().c_str());

        // Reset for next average.
        frameCount = 0;
    }

    frameCount++;
}
