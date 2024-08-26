//
//  GameText.cpp
//  Tetris
//
//  Created by Isha Haider on 2024-08-25.
//

#include <SFML/Graphics.hpp>
#include "Headers/GameText.hpp"
#include <iostream>
#include <filesystem>

void game_text(unsigned short i_x, unsigned short i_y, const std::string& i_text, sf::RenderWindow& i_window)
{
    // Align the text to top left top
    short character_x = i_x;
    short character_y = i_y;
    unsigned char character_width;
//    unsigned char character_height;
    sf::Sprite character_sprite;
    sf::Texture font_texture;
    
    if (!font_texture.loadFromFile("Tetris/Resources/Images/Font.png")) {
        std::cerr << "Failed to load image. Please check the file path and name." << std::endl;
        return;
    }

    // Calculate the width of the character based on the font image size
    // 96 because there are 96 characters in the image
    character_width = font_texture.getSize().x / 96;
//    character_height = font_texture.getSize().y / 8;
    character_sprite.setTexture(font_texture);

    for (const char a : i_text)
    {
        if ('\n' == a)
        {
            //After every newline, increase the y-coordinate and reset the x-coordinate
            character_x = i_x;
//            character_y += character_height;
            character_y += font_texture.getSize().y;
            continue;
        }
        
//        // Calculate the row and column of the character in the texture grid
//        int column = (a - 32) % 10;
//        int row = (a - 32) / 10;

        character_sprite.setPosition(character_x, character_y);
        character_sprite.setTextureRect(sf::IntRect(character_width * (a - 32), 0, character_width, font_texture.getSize().y));
//        character_sprite.setTextureRect(sf::IntRect(column * character_width, row * character_height, character_width, character_height));
        character_x += character_width;
        i_window.draw(character_sprite);
    }
}
