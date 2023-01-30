#pragma once

#include "bitmap_font.h"

class Label
{

public:
    std::string text;
    glm::vec3 color = glm::vec3(0);
    glm::vec2 pos = glm::vec2(0);
    glm::vec2 scale = glm::vec2(1);

    void draw()
    {
        BitmapFont::draw(text, pos, scale, color);
    }

    float getTextWidth()
    {
        return text.size() * scale.x;
    }
};