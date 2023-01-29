#pragma once

#include "../Engine/Core/application.h"
#include "tetromino.h"
#include <vector>

class Board
{
private:
    const uint32_t width = 10;
    const uint32_t height = 20;
    RID grid_line;
    RID next_figure_place;
    RID place_texture;

public:
    std::vector<std::vector<Tile::Color>> board;

    Board() : board(height, std::vector<Tile::Color>(width, Tile::Color::NONE)) {}

    void init()
    {
        grid_line = GPU::gpu.getDeafultQuad();
        next_figure_place = GPU::gpu.getDeafultQuad();
        place_texture = GPU::gpu.createTexture("../tetris/rsc/place.jpg");
        GPU::gpu.textureFilter(place_texture, Texture::Filter::NEAREST);
        GPU::gpu.textureWrapMode(place_texture, Texture::WrapMode::CLAMP_TO_EDGE);
    }

    glm::vec3 grid_color = glm::vec3(0.1);
    float grid_width = 0.015f;
    void draw()
    {
        Program::colored2d.use();
        Program::colored2d.vertexAttrib(VertexAttribs::COLOR, grid_color);

        Transform2d tr;
        tr.scale(glm::vec2(grid_width, height));
        for (int i = 0; i <= width; i++)
        {
            tr.origin = {i, 0};
            Program::colored2d.setPVM(camera2d.getPV() * tr.getMatrix());
            Renderer::drawVAO(grid_line);
        }
        Transform2d tr2;
        tr2.scale(glm::vec2(width, grid_width));
        for (int i = 0; i <= height; i++)
        {
            tr2.origin = {0, i};
            Program::colored2d.setPVM(camera2d.getPV() * tr2.getMatrix());
            Renderer::drawVAO(grid_line);
        }

        Renderer::setMaterial(Tile::matrial);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (board[i][j] != Tile::Color::NONE)
                {
                    Transform2d tr;
                    tr.origin = glm::vec2(j, i);

                    Renderer::setTransform2d(tr.getMatrix());
                    Renderer::drawVAO(Tile::tiles[board[i][j]]);
                }
            }
        }

        Transform2d transform;
        transform.scale({2, 4});
        transform.origin = glm::vec2(width + 2, 4);

        Renderer::useTexture(place_texture);
        Renderer::setTransform2d(transform.getMatrix());
        Renderer::drawVAO(next_figure_place);
    }

    void setPoint(const glm::ivec2 &pos, Tile::Color color)
    {
        board[pos.y][pos.x] = color;
    }

    bool isInside(const glm::ivec2 &pos)
    {
        return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
    }

    bool isEmpty(const glm::ivec2 &pos)
    {
        if (isInside(pos))
            return board[pos.y][pos.x] == Tile::Color::NONE;
        return true;
    }

    void swapLines(int l1, int l2)
    {
        std::swap(board[l1], board[l2]);
    }

    void clearLine(int l)
    {
        for (int i = 0; i < width; i++)
        {
            board[l][i] = Tile::Color::NONE;
        }
    }

    int getW() const { return width; }
    int getH() const { return height; }
};