#include "OpenGLHandler.h"
#include <iostream>
#include "Md2.h"

// Animation constants
namespace
{
    constexpr float MODEL_SCALE = 0.3f;
    constexpr float ROTATION_SPEED = 50.0f;  // degrees per second
    constexpr float ANIMATION_VELOCITY = 5.0f;
}

void display(OpenGLHandler &openGL);

int main()
{
    OpenGLHandler openGL;
    if (!openGL.init())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    display(openGL);
    return 0;
}

void display(OpenGLHandler &openGL)
{
    // Frame numbers and actions is documented in MD2 format
    constexpr int startFrame = 0;
    constexpr int endFrame = 197;

    md2model::Md2 player("data/female.md2", "data/female.tga");
    
    if (!player.isValid())
    {
        std::cerr << "Failed to load MD2 model" << std::endl;
        return;
    }

    double lastTime = glfwGetTime();
    float angle = 0.0f;

    int renderFrame = startFrame;
    float interpolation = 0.0f;

    glm::mat4 view, projection;
    glm::vec3 camPos(0.0f, 0.0f, 0.0f);
    glm::vec3 targetPos(0.0f, 0.0f, -20.0f);
    glm::vec3 up(1.0f, 0.0f, 0.0f);

    // Create the View matrix
    view = glm::lookAt(camPos, camPos + targetPos, up);

    // Create the projection matrix
    projection = glm::perspective(glm::radians(45.0f), (float)OpenGLHandler::getWindowWidth() / (float)OpenGLHandler::getWindowHeight(), 0.1f, 100.0f);

    while (!glfwWindowShouldClose(openGL.getWindow()))
    {
        openGL.showFPS();
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update the model rotation
        if (!OpenGLHandler::isPaused())
        {
            angle += deltaTime * ROTATION_SPEED;
        }

        if (angle >= 360.0f)
        {
            angle = 0.0f;
        }

        // Poll for and process events
        glfwPollEvents();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.Draw(renderFrame, angle, interpolation, view, projection);
        // Swap front and back buffers
        glfwSwapBuffers(openGL.getWindow());

        if (interpolation >= 1.0f)
        {
            interpolation = 0.0f;
            if (renderFrame == endFrame)
            {
                renderFrame = startFrame;
            }
            else
            {
                renderFrame++;
            }
        }
        interpolation += ANIMATION_VELOCITY * deltaTime;
    }
}
