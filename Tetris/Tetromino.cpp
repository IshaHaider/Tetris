//
//  Tetromino.cpp
//  Tetris
//
//  Created by Isha Haider on 2024-08-25.
//


#include <vector>

#include "Headers/Global.hpp"
#include "Headers/GetTetromino.hpp"
#include "Headers/GetWallKickData.hpp"
#include "Headers/Tetromino.hpp"

Tetromino::Tetromino(unsigned char i_shape, const std::vector<std::vector<unsigned char>>& i_matrix) :
    rotation(0),
    shape(i_shape),
    tetros(get_tetromino(i_shape, COLUMNS / 2, 1))
{ }

bool Tetromino::move_down(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    for (Position& tetro : tetros)
    {
        //Will we go outside the matrix if we move down?
        if (ROWS == 1 + tetro.y)
        {
            return 0;
        }

        //Will we hit another tetromino if we move down?
        if (0 < i_matrix[tetro.x][1 + tetro.y])
        {
            return 0;
        }
    }

    //Move the tetromino down
    for (Position& tetro : tetros)
    {
        tetro.y++;
    }

    //Return that everything is okay
    return 1;
}

bool Tetromino::reset(unsigned char i_shape, const std::vector<std::vector<unsigned char>>& i_matrix)
{
    //Reset the variables
    rotation = 0;
    shape = i_shape;

    tetros = get_tetromino(shape, COLUMNS / 2, 1);

    for (Position& tetro : tetros)
    {
        if (0 < i_matrix[tetro.x][tetro.y])
        {
            //Return that we can't reset because there's a tetromino at the spawn location
            return 0;
        }
    }

    //Return that everything is fine
    return 1;
}

unsigned char Tetromino::get_shape()
{
    //I'm gonna take a wild guess and say that this return the shape of the tetromino
    return shape;
}

void Tetromino::hard_drop(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    //I'm so smart. I used the ghost tetromino function to make the hard drop function
    tetros = get_ghost_tetros(i_matrix);
}

void Tetromino::move_left(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    //This is similar to the move down function so I'm not gonna explain anything here. Bite me!
    for (Position& tetro : tetros)
    {
        if (0 > tetro.x - 1)
        {
            return;
        }

        if (0 > tetro.y)
        {
            continue;
        }
        else if (0 < i_matrix[tetro.x - 1][tetro.y])
        {
            return;
        }
    }

    for (Position& tetro : tetros)
    {
        tetro.x--;
    }
}

void Tetromino::move_right(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    //Hello!
    for (Position& tetro : tetros)
    {
        if (COLUMNS == 1 + tetro.x)
        {
            return;
        }

        if (0 > tetro.y)
        {
            continue;
        }
        else if (0 < i_matrix[1 + tetro.x][tetro.y])
        {
            return;
        }
    }

    for (Position& tetro : tetros)
    {
        tetro.x++;
    }
}

void Tetromino::rotate(bool i_clockwise, const std::vector<std::vector<unsigned char>>& i_matrix)
{
    //I don't even wanna explain this. I spent way too much time writing this
    if (3 != shape)
    {
        unsigned char next_rotation;

        std::vector<Position> current_tetros = tetros;

        //Calculating the next rotation state
        if (0 == i_clockwise)
        {
            next_rotation = (3 + rotation) % 4;
        }
        else
        {
            next_rotation = (1 + rotation) % 4;
        }

        //If it's the I shape
        if (0 == shape)
        {
            //We find it's center
            float center_x = 0.5f * (tetros[1].x + tetros[2].x);
            float center_y = 0.5f * (tetros[1].y + tetros[2].y);

            switch (rotation)
            {
                case 0:
                {
                    center_y += 0.5f;

                    break;
                }
                case 1:
                {
                    center_x -= 0.5f;

                    break;
                }
                case 2:
                {
                    center_y -= 0.5f;

                    break;
                }
                case 3:
                {
                    center_x += 0.5f;
                }
            }

            //Then we rotate every tetro based on the center
            for (Position& tetro : tetros)
            {
                //We find the position of the tetro relative to the center
                float x = tetro.x - center_x;
                float y = tetro.y - center_y;

                if (0 == i_clockwise)
                {
                    tetro.x = static_cast<char>(center_x + y);
                    tetro.y = static_cast<char>(center_y - x);
                }
                else
                {
                    tetro.x = static_cast<char>(center_x - y);
                    tetro.y = static_cast<char>(center_y + x);
                }
            }
        }
        else
        {
            //We don't rotate the tetro that's located at the center of rotation
            for (unsigned char a = 1; a < tetros.size(); a++)
            {
                //We find the position of the tetro relative to the central tetro
                char x = tetros[a].x - tetros[0].x;
                char y = tetros[a].y - tetros[0].y;

                if (0 == i_clockwise)
                {
                    tetros[a].x = y + tetros[0].x;
                    tetros[a].y = tetros[0].y - x;
                }
                else
                {
                    tetros[a].x = tetros[0].x - y;
                    tetros[a].y = x + tetros[0].y;
                }
            }
        }

        //We try every vector from the wall kick data
        for (Position& wall_kick : get_wall_kick_data(0 == shape, rotation, next_rotation))
        {
            bool can_turn = 1;

            for (Position& tetro : tetros)
            {
                //Here we're checking every collision that can happen
                if (0 > tetro.x + wall_kick.x || COLUMNS <= tetro.x + wall_kick.x || ROWS <= tetro.y + wall_kick.y)
                {
                    can_turn = 0;

                    break;
                }

                if (0 > tetro.y + wall_kick.y)
                {
                    continue;
                }
                else if (0 < i_matrix[tetro.x + wall_kick.x][tetro.y + wall_kick.y])
                {
                    can_turn = 0;

                    break;
                }
            }

            //If we can turn
            if (1 == can_turn)
            {
                //We turn
                rotation = next_rotation;

                for (Position& tetro : tetros)
                {
                    tetro.x += wall_kick.x;
                    tetro.y += wall_kick.y;
                }

                return;
            }
        }

        //Since we applied changes to the tetros array, we're resetting it when we can't rotate the tetromino
        tetros = current_tetros;
    }
}

void Tetromino::update_matrix(std::vector<std::vector<unsigned char>>& i_matrix)
{
    //Putting the tetromino to the matrix
    for (Position& tetro : tetros)
    {
        if (0 > tetro.y)
        {
            continue;
        }

        i_matrix[tetro.x][tetro.y] = 1 + shape;
    }
}

std::vector<Position> Tetromino::get_ghost_tetros(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    //We're just moving the tetromino down until it hits something. Then we're returning it's position
    bool keep_falling = 1;

    unsigned char total_movement = 0;

    std::vector<Position> ghost_tetros = tetros;

    while (1 == keep_falling)
    {
        total_movement++;

        for (Position& tetro : tetros)
        {
            if (ROWS == total_movement + tetro.y)
            {
                keep_falling = 0;

                break;
            }

            if (0 > total_movement + tetro.y)
            {
                continue;
            }
            else if (0 < i_matrix[tetro.x][total_movement + tetro.y])
            {
                keep_falling = 0;

                break;
            }
        }
    }

    for (Position& tetro : ghost_tetros)
    {
        tetro.y += total_movement - 1;
    }

    return ghost_tetros;
}

std::vector<Position> Tetromino::get_tetros() {
    return tetros;
}
