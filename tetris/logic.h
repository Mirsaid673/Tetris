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

        int dx = 0;
        if (input.getKeyPress(GLFW_KEY_A))
            dx = -1;
        else if (input.getKeyPress(GLFW_KEY_D))
            dx = 1;
        else if (input.getKeyPress(GLFW_KEY_W))
        {
            buffer = tetromino;
            tetromino.rotate();
            if (!check())
            {
                tetromino.translate({1, 0}); // check if we can rotate it but to right
                if (!check())
                {
                    tetromino.translate({-2, 0});
                    if (!check())
                        tetromino = buffer;
                }
            }
        }

        if (dx)
        {
            buffer = tetromino;
            tetromino.translate({dx, 0});
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
            if (tetromino.getMaxY() >= board.getH()) // if out of the board
            {
                gameOver();
                return;
            }
            for (int i = 0; i < 4; i++)
            {
                board.setPoint(tetromino.getPoint(i), tetromino.getColor());
            }
            checkLines(tetromino.getMinY());
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
        tetromino.pos = {board.getW() / 2, board.getH() - 2};
    }

    bool check()
    {
        for (int i = 0; i < 4; i++)
        {
            if (tetromino.getPoint(i).x < 0 || tetromino.getPoint(i).x >= board.getW() ||
                tetromino.getPoint(i).y < 0)
                return false;
            else if (not board.isEmpty(tetromino.getPoint(i)))
                return false;
        }
        return true;
    }

    void checkLines(int minY)
    {
        uint32_t h = board.getH();

        int k = minY;
        for (int i = minY; i < h; i++)
        {
            int count = 0;
            for (int j = 0; j < board.getW(); j++)
            {
                board.board[k][j] = board.board[i][j];
                if (board.board[i][j] != Tile::Color::NONE)
                    count++;
            }

            if (count < board.getW())
                k++;
        }
    }

    void gameOver()
    {
        spdlog::info("game over");
    }
};