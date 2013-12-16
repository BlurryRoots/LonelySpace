
#include <BlurryRoots/Yanecos/IData.hpp>

#include <SFML/System.hpp>

#include <vector>

#ifndef _SPRITESHEETANIMATIONDATA_
#define _SPRITESHEETANIMATIONDATA_

struct SpriteSheetAnimationData
    : public BlurryRoots::Yanecos::IData
{
    std::size_t frameIndex;

    std::size_t frameCount;

    sf::Time duration;

    sf::Time waited;

    sf::IntRect start;

    sf::IntRect cell;

    std::string sheetName;
};

#endif
