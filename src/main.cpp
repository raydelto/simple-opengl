#include "OpenGLHandler.h"
#include <iostream>
#include "Md2.h"

// Animation constants
namespace
{
    constexpr float MODEL_SCALE = 0.3f;
    constexpr float ROTATION_SPEED = 50.0f; // degrees per second
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
    // MD2 Animation and their corresponding frame numbers
    /*
    // Standing / idle
    stand      : 0   - 39

    // Running
    run        : 40  - 45

    // Attacks (standing)
    attack     : 46  - 53     // (sometimes split into attack1 / attack2 / attack3)

    // Pain / damage reactions
    pain1      : 54  - 57
    pain2      : 58  - 61
    pain3      : 62  - 65

    // Jumping
    jump       : 66  - 71

    // Other gestures
    flip       : 72  - 83
    salute     : 84  - 94
    taunt      : 95  - 111
    wave       : 112 - 122
    point      : 123 - 134

    // Crouch idle / move / attack
    cr_stand   : 135 - 153
    cr_walk    : 154 - 159
    cr_attack  : 160 - 168
    cr_pain    : 169 - 172
    cr_death   : 173 - 177

    // Death animations
    death1     : 178 - 183
    death2     : 184 - 189
    death3     : 190 - 197

    */

    // Running animation frames (See the chart above to change animation)
    constexpr int startFrame = 0;
    constexpr int endFrame = 197;

    // Uncomment the lines below one by one to load new models and textures
    md2model::Md2 player("data/cyborg.md2", "data/cyborg1.tga");
    // md2model::Md2 player("data/cyborg.md2", "data/cyborg2.tga");
    // md2model::Md2 player("data/cyborg.md2", "data/cyborg3.tga");
    // md2model::Md2 player("data/female.md2", "data/female.tga");
    // md2model::Md2 player("data/grunt.md2", "data/grunt.tga");
    // md2model::Md2 player("data/tris.md2", "data/tris.tga");

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
            renderFrame = renderFrame == endFrame ? startFrame : renderFrame + 1;
        }
        interpolation += ANIMATION_VELOCITY * deltaTime;
    }
}
