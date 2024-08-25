//
//  main.cpp
//  Tetris
//
//  Created by Isha Haider on 2024-08-24.
//

#include <iostream>
#include <chrono> // C++ Standard Library for time
#include <random>
#include <SFML/Graphics.hpp>

#include "Headers/GameText.hpp"
#include "Headers/Global.hpp"
#include "Headers/GetTetromino.hpp"
#include "Headers/GetWallKickData.hpp"
#include "Headers/Tetromino.hpp"

int main() {
    bool game_over = false;
    bool hard_drop_pressed = false;
    bool rotate_pressed = false;
    unsigned lag = 0; // Used to make the game framerate-independent
    unsigned lines_cleared = 0;
    unsigned char clear_line_timer = 0;
    unsigned char current_fall_speed = START_FALL_SPEED;
    unsigned char fall_timer = 0;
    unsigned char move_timer = 0;
    unsigned char next_shape;
    unsigned char soft_drop_timer = 0;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    //Distribution of all the shapes. Randomly choose one from them
    std::uniform_int_distribution<unsigned short> shape_distribution(0, 6);
    std::vector<bool> clear_rows(ROWS, false);
    std::vector<sf::Color> cell_colors = {
        sf::Color(36, 36, 85),
        sf::Color(0, 219, 255),
        sf::Color(0, 36, 255),
        sf::Color(255, 146, 0),
        sf::Color(255, 219, 0),
        sf::Color(0, 219, 0),
        sf::Color(146, 0, 255),
        sf::Color(219, 0, 0),
        sf::Color(73, 73, 85)
    };
    std::vector<std::vector<unsigned char>> matrix(COLUMNS, std::vector<unsigned char>(ROWS));
    sf:: Event event;
    
    sf::RenderWindow window(sf::VideoMode(2 * CELL_SIZE * COLUMNS * SCREEN_RESIZE, CELL_SIZE * ROWS * SCREEN_RESIZE), "Tetris Game", sf::Style::Close); // 320x640 widthxheight
    window.setView(sf::View(sf::FloatRect(0, 0, 2 * CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));
    
    Tetromino tetromino(static_cast<unsigned char>(shape_distribution(random_engine)), matrix);
    next_shape = static_cast<unsigned char>(shape_distribution(random_engine));
    
    previous_time = std::chrono::steady_clock::now();
    
    while (window.isOpen()) {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
        
        lag += delta_time;
        previous_time += std::chrono::microseconds(delta_time);
        
        // While the lag exceeds the maximum allowed frame duration
        while (FRAME_DURATION <= lag) {
            lag -= FRAME_DURATION;
            
            while (window.pollEvent(event)) {
                // Check the event type
                switch(event.type) {
                    case sf::Event::Closed: {
                        window.close();
                        break;
                    }
                    case sf::Event::KeyReleased: {
                        switch (event.key.code)
                        {
                            case sf::Keyboard::C:
                            case sf::Keyboard::Z:
                            {
                                rotate_pressed = 0; // Rotation key is not pressed anymore
                                break;
                            }
                            case sf::Keyboard::Down:
                            {
                                soft_drop_timer = 0; // Reset the soft drop timer
                                break;
                            }
                            case sf::Keyboard::Left:
                            case sf::Keyboard::Right:
                            {
                                move_timer = 0; // Reset the move timer
                                break;
                            }
                            case sf::Keyboard::Space:
                            {
                                hard_drop_pressed = 0; //Hard drop key is not pressed anymore
                            }
                        }
                    }
                    // REMOVE THESE IF NOT USING
                    case sf::Event::Resized:
                        break;
                    case sf::Event::LostFocus:
                        break;
                    case sf::Event::GainedFocus:
                        break;
                    case sf::Event::TextEntered:
                        break;
                    case sf::Event::KeyPressed:
                        break;
                    case sf::Event::MouseWheelMoved:
                        break;
                    case sf::Event::MouseWheelScrolled:
                        break;
                    case sf::Event::MouseButtonPressed:
                        break;
                    case sf::Event::MouseButtonReleased:
                        break;
                    case sf::Event::MouseMoved:
                        break;
                    case sf::Event::MouseEntered:
                        break;
                    case sf::Event::MouseLeft:
                        break;
                    case sf::Event::JoystickButtonPressed:
                        break;
                    case sf::Event::JoystickButtonReleased:
                        break;
                    case sf::Event::JoystickMoved:
                        break;
                    case sf::Event::JoystickConnected:
                        break;
                    case sf::Event::JoystickDisconnected:
                        break;
                    case sf::Event::TouchBegan:
                        break;
                    case sf::Event::TouchMoved:
                        break;
                    case sf::Event::TouchEnded:
                        break;
                    case sf::Event::SensorChanged:
                        break;
                    case sf::Event::Count:
                        break;
                }
            }
            
            if (!clear_line_timer) {
                if (!game_over) {
                    if (!rotate_pressed) {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                            rotate_pressed = true;
                            tetromino.rotate(1, matrix); // barrel roll
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                            rotate_pressed = true;
                            tetromino.rotate(0, matrix); // barrel roll
                        }
                    }
                    if (!move_timer) {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                            move_timer = true;
                            tetromino.move_left(matrix);
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                            move_timer = true;
                            tetromino.move_right(matrix);
                        }
                    }
                    else { move_timer = (1 + move_timer) % MOVE_SPEED; }
                    
                    if (!hard_drop_pressed) {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                            hard_drop_pressed = true;
                            fall_timer = current_fall_speed;
                            tetromino.hard_drop(matrix);
                        }
                    }
                    
                    if (!soft_drop_timer) {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                            if (tetromino.move_down(matrix)) {
                                fall_timer = 0;
                                soft_drop_timer = 1;
                            }
                        }
                    }
                    else { soft_drop_timer = (1 + soft_drop_timer) % SOFT_DROP_SPEED; }
                    
                    if (current_fall_speed == fall_timer) {
                        if (!tetromino.move_down(matrix)) {
                            tetromino.update_matrix(matrix);
                            for (unsigned char i = 0; i < ROWS; i++) {
                                bool clear_line = true;
                                for (unsigned char j = 0; j < COLUMNS; j++) {
                                    if (!matrix[j][i]) {
                                        clear_line = false;
                                        break;
                                    }
                                }
                                
                                if (clear_line) {
                                    lines_cleared++;
                                    clear_line_timer = CLEAR_LINE_DURATION;
                                    clear_rows[i] = true;
                                    if ((lines_cleared % LINES_TO_INCREASE_SPEED) == 0) {
                                        current_fall_speed = std::max<unsigned char>(SOFT_DROP_SPEED, current_fall_speed - 1);
                                    }
                                }
                            }
                            
                            if (!clear_line_timer) {
                                game_over = !tetromino.reset(next_shape, matrix);
                                next_shape = static_cast<unsigned char>(shape_distribution(random_engine));
                            }
                        }
                        fall_timer = 0;
                    }
                    else { fall_timer++; }
                }
                
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    game_over = false;
                    hard_drop_pressed = false;
                    rotate_pressed = false;
                    lines_cleared = false;
                    
                    current_fall_speed = START_FALL_SPEED;
                    fall_timer = 0;
                    move_timer = 0;
                    soft_drop_timer = 0;
                    
                    for (std::vector<unsigned char>& a : matrix) {
                        std::fill(a.begin(), a.end(), 0);
                    }
                }
            }
            else {
                clear_line_timer--;
                if (clear_line_timer == 0) {
                    for (unsigned char i = 0; i < ROWS; i++) {
                        if (clear_rows[i]) {
                            for (unsigned char j = 0; j < COLUMNS; j++) {
                                matrix[j][i] = 0;
                                for (unsigned char k = i; 0 < k; k--) {
                                    matrix[j][k] = matrix[j][k - 1];
                                    matrix[j][k - 1] = 0;
                                }
                            }
                        }
                    }
                    
                    game_over = !tetromino.reset(next_shape, matrix);
                    next_shape = static_cast<unsigned char>(shape_distribution(random_engine));
                    std::fill(clear_rows.begin(), clear_rows.end(), 0);
                }
            }
            
            if (FRAME_DURATION > lag) {
                unsigned char clear_cell_size = static_cast<unsigned char>(2 * round(0.5f * CELL_SIZE * (clear_line_timer / static_cast<float>(CLEAR_LINE_DURATION))));
                
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                sf::RectangleShape next_shape_border(sf::Vector2f(5 * CELL_SIZE, 5 * CELL_SIZE));
                next_shape_border.setFillColor(sf::Color(0,0,0));
                next_shape_border.setOutlineThickness(-1);
                next_shape_border.setPosition(CELL_SIZE * (1.5f * COLUMNS - 2.5f), CELL_SIZE * (0.25f * ROWS - 2.5f));
                
                window.clear();
                
                for (unsigned char i = 0; i < COLUMNS; i++) {
                    for (unsigned char j = 0; j < ROWS; j++) {
                        if (!clear_rows[j]) {
                            cell.setPosition(static_cast<float>(CELL_SIZE * i), static_cast<float>(CELL_SIZE * j));
                            if (game_over && 0 < matrix[i][j]) { cell.setFillColor(cell_colors[8]); }
                            else { cell.setFillColor(cell_colors[matrix[i][j]]); }
                            window.draw(cell);
                        }
                    }
                }
                cell.setFillColor(cell_colors[8]);
                
                if (!game_over) {
                    for (Position& tetro : tetromino.get_ghost_tetros(matrix)) {
                        cell.setPosition(static_cast<float>(CELL_SIZE * tetro.x), static_cast<float>(CELL_SIZE * tetro.y));
                        window.draw(cell);
                    }
                    cell.setFillColor(cell_colors[1 + tetromino.get_shape()]);
                }
                
                for (Position& tetro : tetromino.get_tetros()) {
                    cell.setPosition(static_cast<float>(CELL_SIZE * tetro.x), static_cast<float>(CELL_SIZE * tetro.y));
                    window.draw(cell);
                }
                
                for (unsigned char i = 0; i < COLUMNS; i++) {
                    for (unsigned char j = 0; j < ROWS; j++) {
                        if (clear_rows[j]) {
                            cell.setFillColor(cell_colors[0]);
                            cell.setPosition(static_cast<float>(CELL_SIZE * i), static_cast<float>(CELL_SIZE * j));
                            cell.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                            window.draw(cell);
                            
                            cell.setFillColor(sf::Color(255, 255, 255));
                            cell.setPosition(floor(CELL_SIZE * (0.5f + i) - 0.5f * clear_cell_size), floor(CELL_SIZE * (0.5f + j) - 0.5f * clear_cell_size));
                            cell.setSize(sf::Vector2f(clear_cell_size, clear_cell_size));
                            window.draw(cell);
                        }
                    }
                }
                
                cell.setFillColor(cell_colors[1 + next_shape]);
                cell.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                window.draw(next_shape_border);
                
                for (Position& tetro : get_tetromino(next_shape, static_cast<unsigned char>(1.5f * COLUMNS), static_cast<unsigned char>(0.25f * ROWS))) {
                    unsigned short next_tetro_x = CELL_SIZE * tetro.x;
                    unsigned short next_tetro_y = CELL_SIZE * tetro.y;
                    
                    if (next_shape == 0){ next_tetro_y += static_cast<unsigned char>(round(0.5f * CELL_SIZE)); }
                    else if (next_shape != 3) { next_tetro_x -= static_cast<unsigned char>(round(0.5f * CELL_SIZE)); }
                    
                    cell.setPosition(next_tetro_x, next_tetro_y);
                    window.draw(cell);
                }
                
                game_text(
                  static_cast<unsigned short>(CELL_SIZE * (0.5f + COLUMNS)),
                  static_cast<unsigned short>(0.5f * CELL_SIZE * ROWS),
                  "Lines:" + std::to_string(lines_cleared) + "\nSpeed:" + std::to_string(START_FALL_SPEED / current_fall_speed) + 'x',
                  window
                );
                
                window.display();
            }
        }
    }
}
