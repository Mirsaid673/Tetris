#pragma once

#include "../Engine/Core/application.h"
#include "tile.h"
#include "board.h"
#include "logic.h"

class Game : public Application
{
    Logic logic;

    void init() override
    {
        camera2d.ortho(0, scale, scale * (float)window.getHeight() / (float)window.getWidth(), 0);
        Renderer::setClearColor(glm::vec3(0.25, 0.2f, 0.3f));
        Tile::init();

        scene.addNode(logic);
    }

    Material m;
    void draw() override
    {
    }

    float scale = 14.0f;
    void onResize() override
    {
        camera2d.ortho(0, scale, scale * (float)window.getHeight() / (float)window.getWidth(), 0);
    }

    void update() override
    {
        if (input.getKeyDown(GLFW_KEY_ESCAPE))
            running = false;
    }
};