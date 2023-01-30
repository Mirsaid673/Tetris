#pragma once

#include "board.h"
#include "tetromino.h"
#include "label.h"
#include <glm/gtc/random.hpp>

class Logic : public Node
{
private:
    Board board;
    Tetromino tetromino;
    Tetromino buffer;

public:
    uint32_t score = 0;
    float default_dealy = 0.5f;
    float delay;

    int erased_lines_count = 0;

    float current_time = 0.0f;
    void update() override
    {
        delay = default_dealy;
        if (input.getKeyDown(GLFW_KEY_S) || input.getKeyDown(GLFW_KEY_DOWN))
            delay = 0.05f;

        current_time += Time::delta_time;
        if (current_time >= delay)
        {
            current_time -= delay;
            tick();
        }

        int dx = 0;
        if (input.getKeyPress(GLFW_KEY_A) || input.getKeyPress(GLFW_KEY_LEFT))
            dx = -1;
        else if (input.getKeyPress(GLFW_KEY_D) || input.getKeyPress(GLFW_KEY_RIGHT))
            dx = 1;
        else if (input.getKeyPress(GLFW_KEY_W) || input.getKeyPress(GLFW_KEY_UP))
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
            spawn();
        }
    }

    Tetromino next;
    void init() override
    {
        board.init();
        current_rand = getRandomNum();
        next_rand = getRandomNum();
        spawn();

        next.pos = {board.getW() + 1, 1};

        score_counter.scale = glm::vec2(1.0f / BitmapFont::getAspect(), 1) * glm::vec2(0.7f);
        score_counter.text = "0";
        score_counter.pos.y = 8;
        rightLabel();

        score_label.scale = glm::vec2(1.0f / BitmapFont::getAspect(), 1) * glm::vec2(1);
        score_label.pos = {board.getW(), 8.5f};
        score_label.text = "SCORE:";
    }

    float sscore_counterX = 14;
    void rightLabel()
    {
        score_counter.pos.x = sscore_counterX - score_counter.getTextWidth();
    }

    Label score_label;
    Label score_counter;
    void draw() override
    {
        board.draw();
        tetromino.draw();
        next.draw();

        score_label.draw();
        score_counter.draw();
    }

    int current_rand;
    int next_rand;
    void spawn()
    {
        current_rand = next_rand;
        next_rand = getRandomNum();

        tetromino.setType((Tetromino::Type)(current_rand));
        tetromino.pos = {board.getW() / 2, board.getH() - 1};

        next.setType((Tetromino::Type)(next_rand));
    }

    int getRandomNum()
    {
        return glm::linearRand(0, 6);
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

        int erased_lines = 0;
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
            else
                erased_lines++;
        }
        if (erased_lines)
        {
            erased_lines_count += erased_lines;

            if (erased_lines_count % 20 == 0)
                default_dealy *= 0.90;

            switch (erased_lines)
            {
            case 1:
                score += 100;
                break;
            case 2:
                score += 300;
                break;
            case 3:
                score += 700;
                break;
            case 4:
                score += 1500;
                break;
            }
            onLineErase();
        }
    }

    void onLineErase()
    {
        score_counter.text = std::to_string(score);
        rightLabel();
    }

    bool game_over = false;
    void gameOver()
    {
        spdlog::info("game over, your score: {}", score);
        game_over = true;
    }
};