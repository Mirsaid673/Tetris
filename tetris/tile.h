#pragma once

#include "../Engine/Core/application.h"

namespace Tile
{
    enum Color : uint8_t
    {
        RED,
        GREEN,
        BLUE,
        YELLOW,
        ORANGE,
        PURPLE,
        CYAN,
        NONE,
    };

    const int TILES_COUNT = 7;
    RID tiles[TILES_COUNT];
    Material matrial;
    void init()
    {
        matrial.program = &Program::basic2d;
        matrial.texture = GPU::gpu.createTexture("../tetris/rsc/tiles.png");
        GPU::gpu.textureFilter(matrial.texture, Texture::Filter::NEAREST);
        GPU::gpu.textureWrapMode(matrial.texture, Texture::WrapMode::CLAMP_TO_EDGE);

        glm::vec2 left_bottom(0);
        glm::vec2 right_top(1);

        float part = 1.0f / TILES_COUNT;

        glm::vec2 uv_left_bottom(0);
        glm::vec2 uv_right_top(part, 1);

        for (int i = 0; i < TILES_COUNT; i++)
        {
            tiles[i] = GPU::gpu.loadMesh(
                ResourceManager::createQuadMesh(left_bottom, right_top, uv_left_bottom, uv_right_top));

            uv_left_bottom.x += part;
            uv_right_top.x += part;
        }
    }
};