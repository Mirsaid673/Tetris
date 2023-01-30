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
        std::srand((unsigned)time(nullptr));
        camera2d.ortho(0, scale, scale * (float)window.getHeight() / (float)window.getWidth(), 0);
        Renderer::setClearColor(glm::vec3(0.25, 0.2f, 0.3f));
        Tile::init();
        BitmapFont::init("../tetris/rsc/font.png", 16, 8);

        scene.addNode(logic);
    }

    float scale = 14.0f;
    void onResize() override
    {
        camera2d.ortho(0, scale, scale * (float)window.getHeight() / (float)window.getWidth(), 0);
    }

    void update() override
    {
        if (input.getKeyDown(GLFW_KEY_ESCAPE) || logic.game_over)
            running = false;
        window.setTitle((std::string(window.getTitle()) + "   " + std::to_string(1.0f / Time::delta_time)).c_str());
    }

    void determinate() override
    {
        BitmapFont::destroy();
    }
};