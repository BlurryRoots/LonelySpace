
#include <BlurryRoots/Yanecos/IData.hpp>

#include <SFML/Graphics.hpp>

#include <memory>

#ifndef _TEXT_DATA_
#define _TEXT_DATA_

struct TextData
    : public BlurryRoots::Yanecos::IData
{
    std::shared_ptr<sf::Font> font;
    
    std::string text;
    
    sf::Color color;
};

#endif
