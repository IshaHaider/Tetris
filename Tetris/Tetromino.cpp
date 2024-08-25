//
//  Tetromino.cpp
//  Tetris
//
//  Created by Isha Haider on 2024-08-25.
//


#include <vector>

#include "Headers/Global.hpp"
#include "Headers/GetTetromino.hpp"
//#include "Headers/GetWallKickData.hpp"
#include "Headers/Tetromino.hpp"

Tetromino::Tetromino(unsigned char i_shape, const std::vector<std::vector<unsigned char>>& i_matrix) :
    rotation(0),
    shape(i_shape),
    minos(get_tetromino(i_shape, COLUMNS / 2, 1))
{
    
}
