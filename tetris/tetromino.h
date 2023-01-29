#pragma once

#include "tile.h"

class Tetromino
{
private:
    RID tile;

    Tile::Color color;

public:
    glm::ivec2 points[4];
    glm::ivec2 pos = glm::ivec2(0);
    enum Type : uint8_t
    {
        I,
        Z,
        S,
        T,
        L,
        J,
        O,
    };

    static int figures[7][4];
    void setType(Type t)
    {
        tile = Tile::tiles[t];
        color = (Tile::Color)t;
        int *figure = figures[(size_t)t];

        for (int i = 0; i < 4; i++)
        {
            points[i].x = figure[i] % 2;
            points[i].y = figure[i] / 2;
        }
    }

    void draw()
    {
        Renderer::setMaterial(Tile::matrial);

        for (int i = 0; i < 4; i++)
        {
            Transform2d tr;
            tr.origin = points[i] + pos;

            Renderer::setTransform2d(tr.getMatrix());
            Renderer::drawVAO(tile);
        }
    }

    void translate(const glm::ivec2 &dir)
    {
        pos += dir;
    }

    void rotate()
    {
        glm::mat2 r(0, 1, -1, 0);
        for (int i = 0; i < 4; i++)
        {
            points[i] = (points[i] - glm::ivec2(2, 1)) * r;
        }
    }
    Tile::Color getColor() const { return color; }

    Tetromino() = default;
    Tetromino(Type t) { setType(t); }

    int getMinY()
    {
        return glm::min(points[0].y, glm::min(points[1].y, glm::min(points[2].y, points[3].y))) + pos.y;
    }
    int getMaxY()
    {
        return glm::max(points[0].y, glm::max(points[1].y, glm::max(points[2].y, points[3].y))) + pos.y;
    }

    glm::ivec2 getPoint(int i) const
    {
        return points[i] + pos;
    }
};

int Tetromino::figures[7][4]{
    0, 2, 4, 6, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5, // O
};