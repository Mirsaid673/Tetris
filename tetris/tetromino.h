#pragma once

#include "tile.h"

class Tetromino
{
private:
    RID tile;

    Tile::Color color;

public:
    glm::ivec2 points[4];
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
            tr.origin = points[i];

            Renderer::setTransform2d(tr.getMatrix());
            Renderer::drawVAO(tile);
        }
    }

    void translate(const glm::ivec2 &dir)
    {
        for (int i = 0; i < 4; i++)
            points[i] += dir;
    }

    void rotate()
    {
        glm::ivec2 p = points[2];
        for (int i = 0; i < 4; i++)
        {
            glm::ivec2 r = points[i] - p;
            points[i] = p + glm::ivec2(r.y, -r.x);
        }
    }
    Tile::Color getColor() const { return color; }

    Tetromino() = default;
    Tetromino(Type t) { setType(t); }

    int getMinY()
    {
        return glm::min(points[0].y, glm::min(points[1].y, glm::min(points[2].y, points[3].y)));
    }
    int getMaxY()
    {
        return glm::max(points[0].y, glm::max(points[1].y, glm::max(points[2].y, points[3].y)));
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