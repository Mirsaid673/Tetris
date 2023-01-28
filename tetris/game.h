#pragma once

#include "../Engine/Core/application.h"

class Game : public Application
{

    void init() override
    {
        camera2d.ortho(1, 2);
        Renderer::setClearColor(glm::vec3(0.5));
    }

    void draw() override
    {
    }

    void determinate() override
    {
    }

    void onResize() override
    {
    }

    void update() override
    {
        if (input.getKeyDown(GLFW_KEY_ESCAPE))
            running = false;
    }
};