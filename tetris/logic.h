#pragma once

#include "board.h"
#include "tetromino.h"
#include <glm/gtc/random.hpp>

class Logic : public Node
{
private:
    Board board;
    Tetromino tetromino;
    Tetromino buffer;

public:
    float default_dealy = 0.5f;
    float delay;

    float current_time = 0.0f;
    void update() override
    {
        delay = default_dealy;
        if (input.getKeyDown(GLFW_KEY_S))
            delay = 0.1f;

        current_time += Time::delta_time;
        if (current_time >= delay)
        {
            current_time -= delay;
            tick();
        }

        if (input.getKeyPress(GLFW_KEY_A))
        {
            buffer = tetromino;
            tetromino.translate({-1, 0});
            if (!check())
                tetromino = buffer;
        }
        else if (input.getKeyPress(GLFW_KEY_D))
        {
            buffer = tetromino;
            tetromino.translate({1, 0});
            if (!check())
                tetromino = buffer;
        }
        else if (input.getKeyPress(GLFW_KEY_W))
        {
            buffer = tetromino;
            tetromino.rotate();
            if (!check())
                tetromino = buffer;
        }
    }

    void tick()
    {
        buffer = tetromino;
        tetromino.translate({0, -1});
        if (!check())
        {
            tetromino = buffer;
            for (int i = 0; i < 4; i++)
            {
                board.setPoint(tetromino.points[i], tetromino.getColor());
            }
            checkLines(tetromino.getMinY(), tetromino.getMaxY());
            rand();
        }
    }

    void init() override
    {
        rand();
    }

    void draw() override
    {
        board.draw();
        tetromino.draw();
    }

    void rand()
    {
        tetromino.setType((Tetromino::Type)(glm::linearRand(0, 1024) % 7));
        tetromino.translate({board.getW() / 2, board.getH() - 2});
    }

    bool check()
    {
        for (int i = 0; i < 4; i++)
        {
            if (tetromino.points[i].x < 0 || tetromino.points[i].x >= board.getW() ||
                tetromino.points[i].y < 0)
                return false;
            else if (not board.isEmpty(tetromino.points[i]))
                return false;
        }
        return true;
    }

    void checkLines(int minY, int maxY)
    {
        uint32_t last = board.getLastNotEmptyLine();


        for (int i = minY; i <= maxY; i ++)
        {
            if(board.isLineFull(i))
                board.clearLine(i);
        }

        for (int i = minY; i <= maxY;)
        {
            if (board.isLineEmpty(i))
            {
                for (int j = i; j < board.getH() - 1; j++)
                    board.swapLines(j, j + 1);
            }
            else
                i++;
        }
    }
};