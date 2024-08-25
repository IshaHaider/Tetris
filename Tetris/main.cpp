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

//#include "Headers/DrawText.hpp"
#include "Headers/Global.hpp"
#include "Headers/GetTetromino.hpp"
//#include "Headers/GetWallKickData.hpp"
#include "Headers/Tetromino.hpp"

int main() {
    bool game_over = false;
    bool hard_drop_pressed = false;
    bool rotate_pressed = false;
    unsigned lag = false; // Used to make the game framerate-independent
    unsigned lines_cleared = false;
    unsigned char clear_effect_timer = false;
    unsigned char current_fall_speed = START_FALL_SPEED;
    unsigned char fall_timer = false;
    unsigned char move_timer = false;
    unsigned char next_shape;
    unsigned char soft_drop_timer = false;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    //Distribution of all the shapes. Randomly choose one from them
    std::uniform_int_distribution<unsigned short> shape_distribution(0, 6);
    std::vector<bool> clear_lines(ROWS, 0);
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
            
            if (!clear_effect_timer) {
                
            }
            else {
                
            }
            
            if (FRAME_DURATION > lag){
                
            }
            window.clear();
            window.display();
        }
    }
}
