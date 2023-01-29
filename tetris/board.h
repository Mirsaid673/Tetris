#pragma once

#include "../Engine/Core/application.h"
#include "tetromino.h"
#include <vector>

class Board
{
private:
    const uint32_t width = 10;
    const uint32_t height = 20;

public:
    std::vector<std::vector<Tile::Color>> board;

    Board() : board(height, std::vector<Tile::Color>(width, Tile::Color::NONE)) {}

    void draw()
    {
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