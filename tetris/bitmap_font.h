#pragma once

#include "../Engine/Core/application.h"

namespace BitmapFont
{
    Program font_program;
    Material material;
    glm::ivec2 tex_size;
    int columns;
    int rows;
    RID quad;

    void init(const char *font_path, int font_columns, int font_rows)
    {
        font_program.create("../tetris/rsc/text.vert", "../tetris/rsc/text.frag");

        material.program = &font_program;
        material.texture = GPU::gpu.createTexture(font_path);
        GPU::gpu.textureFilter(material.texture, Texture::Filter::NEAREST);
        GPU::gpu.textureWrapMode(material.texture, Texture::WrapMode::CLAMP_TO_EDGE);

        quad = GPU::gpu.loadMesh(ResourceManager::createQuadMesh({0, 0}, {1, 1}));

        columns = font_columns;
        rows = font_rows;

        tex_size = GPU::gpu.getTexture(material.texture).getSize();
    }

    void destroy()
    {
        font_program.destroy();
    }

    void draw(const std::string &text, const glm::vec2 &pos = {0, 0}, const glm::vec2 &scale = {1, 1}, const glm::vec3 &color = {0, 0, 0})
    {
        Renderer::setMaterial(material);

        glm::vec2 part = glm::vec2(1.0f / (float)columns, 1.0f / (float)rows);
        font_program.setVec2("tex_coord_scale", part);
        font_program.setVec3("color", color);

        Transform2d transform;
        transform.origin = pos;
        transform.scale(scale);

        for (int i = 0; i < text.size(); i++)
        {
            char character = text[i];
            glm::ivec2 s = glm::ivec2(character % columns, rows - 1 - character / columns);

            glm::vec2 offset = part * glm::vec2(s);

            font_program.setVec2("tex_coord_offset", offset);

            Renderer::setTransform2d(transform.getMatrix());
            Renderer::drawVAO(quad);

            transform.origin += glm::vec2(scale.x, 0.0f);
        }
    }

    float getAspect()
    {
        return ((float)(tex_size.y) / (float)rows) / ((float)tex_size.x / (float)columns);
    }
};